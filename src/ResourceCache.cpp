#include "unicore/ResourceCache.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/FileProvider.hpp"
#include "unicore/RendererResource.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCreator.hpp"
#include "unicore/ResourceConverter.hpp"

namespace unicore
{
	static auto& s_resource_type = get_type<Resource>();

	ResourceCache::ResourceCache(Logger& logger, ReadFileProvider& provider)
		: _logger(logger), _provider(provider)
	{
	}

	void ResourceCache::unload_all()
	{
		_cached.clear();
		_resources.clear();
	}

	void ResourceCache::unload_unused()
	{
		for (auto& [path, dict] : _cached)
		{
			for (auto it = dict.begin(); it != dict.end();)
			{
				if (it->second.use_count() == 1)
				{
					UC_LOG_DEBUG(_logger) << "Unload resource " << *it->first << " from " << path;
					it = dict.erase(it);
				}
				else ++it;
			}
		}

		// Remove expired weak pointers
		for (auto it = _resources.begin(); it != _resources.end();)
		{
			if (it->use_count() > 0) ++it;
			else it = _resources.erase(it);
		}
	}

	Shared<Resource> ResourceCache::find(const Path& path, TypeConstRef type) const
	{
		const auto it = _cached.find(path);
		if (it == _cached.end()) return nullptr;

		for (const auto& [res_type, resource] : it->second)
		{
			if (res_type == &type || res_type->is_derived_from(type))
				return resource;
		}

		return nullptr;
	}

	Optional<Path> ResourceCache::find_path(const Resource& resource) const
	{
		for (const auto& it : _cached)
		{
			for (const auto& jt : it.second)
			{
				if (jt.second.get() == &resource)
					return it.first;
			}
		}

		return std::nullopt;
	}

	Shared<Resource> ResourceCache::create(TypeConstRef type, const std::any& value)
	{
		if (value.type() == typeid(Path))
		{
			const auto path = std::any_cast<Path>(value);
			return load(path, type);
		}

		const auto it = _creators.find(&type);
		if (it == _creators.end())
		{
			UC_LOG_WARNING(_logger) << "No creator added for " << type;
			return nullptr;
		}

		const ResourceCreator::Options context{ *this, &_logger };

		for (const auto& creator : it->second)
		{
			if (!creator->can_create(value))
				continue;

			if (auto resource = creator->create(context, value); resource != nullptr)
			{
				UC_LOG_DEBUG(_logger) << "Created " << type << " from " << value.type().name()
					<< " " << MemorySize{ resource->get_system_memory_use() };
				_resources.push_back(resource);
				return resource;
			}

			UC_LOG_ERROR(_logger) << "Failed to load " << type
				<< " with creator " << creator->type()
				<< " with value" << value.type();
		}

		UC_LOG_WARNING(_logger) << "No " << type << " creator found for '"
			<< value.type().name() << "' value type";

		return nullptr;
	}

	Shared<Resource> ResourceCache::load(const Path& path, TypeConstRef type, ResourceCacheFlags flags)
	{
		if (auto resource_find = find(path, type))
			return resource_find;

		const auto logger = !flags.has(ResourceCacheFlag::Quiet) ? &_logger : nullptr;

		if (auto resource = load_resource(path, type, logger))
			return resource;

		if (const auto it = _converters.find(&type); it != _converters.end())
		{
			for (const auto& converter : it->second)
			{
				// TODO: Handle cyclic loading
				if (auto raw = load(path, converter->raw_type(), flags))
				{
					if (auto resource = converter->convert(*raw, { *this, logger }))
					{
						// TODO: Optimize (get path directly from load function)
						const auto converted_path = find_path(*raw);
						_resources.push_back(resource);
						add_resource(resource, converted_path.value_or(path));
						return resource;
					}
				}
			}
		}

		UC_LOG_WARNING(logger) << "Failed to load " << type << " from " << path;
		return nullptr;
	}

	void ResourceCache::dump_used()
	{
		unsigned index = 0;
		MemorySize sys_mem{ 0 };

		UC_LOG_INFO(_logger) << "Used resource dump";
		UC_LOG_INFO(_logger) << "----------------------------------";

		for (const auto& weak : _resources)
		{
			if (const auto resource = weak.lock())
			{
				const auto memory_use = MemorySize{ resource->get_system_memory_use() };
				sys_mem += memory_use;

				auto path = find_path(*resource);

				UC_LOG_INFO(_logger) << index << ": "
					<< (path.has_value() ? path.value() : Path::Empty)
					<< " " << resource->type()
					<< " [" << resource.use_count()
					<< ", " << memory_use << "]";
				index++;
			}
		}

		UC_LOG_INFO(_logger) << "----------------------------------";
		UC_LOG_INFO(_logger) << "Used system memory: " << sys_mem;
	}

	void ResourceCache::calc_memory_use(size_t* system, size_t* video) const
	{
		if (system == nullptr && video == nullptr) return;

		if (system != nullptr)
			*system = 0;

		if (video != nullptr)
			*video = 0;

		for (const auto& it : _cached)
		{
			for (const auto& [_, resource] : it.second)
			{
				if (system != nullptr)
					*system += resource->get_system_memory_use();
				if (video != nullptr)
				{
					if (const auto render_resource = std::dynamic_pointer_cast<RendererResource>(resource))
						*video += render_resource->get_video_memory_use();
				}
			}
		}
	}

	void ResourceCache::add_loader(const Shared<ResourceLoader>& loader)
	{
		for (auto type = &loader->resource_type(); type != nullptr && type != &s_resource_type; type = type->parent)
		{
			_loaders[type].insert(loader);
		}
	}

	void ResourceCache::add_converter(const Shared<ResourceConverter>& converter)
	{
		_converters[&converter->resource_type()].insert(converter);
	}

	void ResourceCache::add_creator(const Shared<ResourceCreator>& creator)
	{
		for (auto type = &creator->resource_type(); type != nullptr && type != &s_resource_type; type = type->parent)
			_creators[type].insert(creator);
	}

	void ResourceCache::unregister_module(const ModuleContext& context)
	{
		Module::unregister_module(context);

		_loaders.clear();
		_converters.clear();
		_creators.clear();

		unload_all();
	}

	// ============================================================================
	bool ResourceCache::LoaderSort::operator()(const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const
	{
		if (lhs->priority() == rhs->priority())
			return lhs < rhs;
		return lhs->priority() < rhs->priority();
	}

	bool ResourceCache::CreatorSort::operator()(const Shared<ResourceCreator>& lhs, const Shared<ResourceCreator>& rhs) const
	{
		if (lhs->priority() == rhs->priority())
			return lhs < rhs;
		return lhs->priority() < rhs->priority();
	}

	Shared<Resource> ResourceCache::load_resource(const Path& path, TypeConstRef type, Logger* logger)
	{
		const auto it = _loaders.find(&type);
		if (it == _loaders.end()) return nullptr;

		const auto& loaders = it->second;
		// TODO: Implement loading stack for prevent recursive loading
		for (const auto& loader : loaders)
		{
			if (path.has_extension(Path::WildcardExt))
			{
				for (const auto& extension : loader->extension())
				{
					Path new_path(path);
					new_path.replace_extension(extension);

					const auto stream = _provider.open_read(new_path);
					if (!stream)
						continue;

					if (auto resource = load_resource(*loader, new_path, *stream, logger))
						return resource;
				}
			}
			else
			{
				const auto extension = path.extension();

				const auto stream = _provider.open_read(path);
				if (!stream)
				{
					UC_LOG_WARNING(logger) << "Failed to open " << path;
					return nullptr;
				}

				if (auto resource = load_resource(*loader, path, *stream, logger))
					return resource;
			}
		}

		return nullptr;
	}

	Shared<Resource> ResourceCache::load_resource(ResourceLoader& loader,
		const Path& path, ReadFile& file, Logger* logger)
	{
		if (auto resource = loader.load({ path, *this, file, logger }))
		{
			_resources.push_back(resource);
			add_resource(resource, path);

			return resource;
		}

		return nullptr;
	}

	bool ResourceCache::add_resource(const Shared<Resource>& resource, const Path& path)
	{
		if (resource->cache_policy() == ResourceCachePolicy::CanCache)
		{
			const auto& type = resource->type();
			_cached[path].insert_or_assign(&type, resource);
			UC_LOG_DEBUG(_logger) << "Added " << type << " from " << path
				<< " " << MemorySize{ resource->get_system_memory_use() };
			return true;
		}

		return false;
	}
}