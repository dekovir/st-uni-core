#include "unicore/ResourceCache.hpp"
#include "unicore/Math.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/FileProvider.hpp"
#include "unicore/RendererResource.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct FromPath
	{
		explicit FromPath(const Path& path_) : path(path_) {}
		const Path& path;
	};

	struct WithOptions
	{
		explicit WithOptions(const ResourceOptions* options_) : options(options_) {}
		const ResourceOptions* options;
	};

	UNICODE_STRING_BUILDER_FORMAT(const FromPath&)
	{
		if (!value.path.empty())
			return builder << " from " << value.path;
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const WithOptions&)
	{
		if (value.options != nullptr)
			return builder << " with " << typeid(*value.options);
		return builder;
	}

	ResourceCache::ResourceCache(Logger& logger)
		: _logger(logger)
	{
	}

	void ResourceCache::unload_all()
	{
		_cached.clear();
		_resources.clear();
	}

	void ResourceCache::unload_unused()
	{
		for (auto& [_, cached] : _cached)
		{
			for (auto jt = cached.begin(); jt != cached.end();)
			{
				auto& info = jt->second;
				if (info.resource.use_count() == 1)
				{
					UC_LOG_DEBUG(_logger) << "Unload resource "
						<< info.resource->type() << " from " << info.path;
					jt = cached.erase(jt);
				}
				else ++jt;
			}
		}

		// Remove expired weak pointers
		for (auto it = _resources.begin(); it != _resources.end();)
		{
			if (it->use_count() > 0) ++it;
			else it = _resources.erase(it);
		}
	}

	Optional<Path> ResourceCache::find_path(const Resource& resource) const
	{
		for (const auto& it : _cached)
		{
			for (const auto& jt : it.second)
			{
				auto& info = jt.second;
				if (info.resource.get() == &resource)
					return info.path;
			}
		}

		return std::nullopt;
	}

	Shared<Resource> ResourceCache::create(TypeConstRef type, const ResourceOptions& options)
	{
		return load(Path::Empty, type, &options, ResourceCacheFlags::Zero);
	}

	Shared<Resource> ResourceCache::load(const Path& path,
		TypeConstRef type, const ResourceOptions* options, ResourceCacheFlags flags)
	{
		const auto logger = !flags.has(ResourceCacheFlag::Quiet) ? &_logger : nullptr;

		const auto loaders_it = _loaders.find(&type);
		if (loaders_it == _loaders.end())
		{
			UC_LOG_ERROR(logger) << "No loaders for " << type;
			return nullptr;
		}

		// TODO: Implement loading stack for prevent recursive loading
		const auto& loaders = loaders_it->second;
		const auto extension = path.extension();
		const size_t hash = make_hash(path, options);

		for (const auto& loader : loaders)
		{
			if (!loader->can_load(path))
				continue;

			if (!loader->can_load(options))
				continue;

			if (const auto it = _cached.find(loader.get()); it != _cached.end())
			{
				for (const auto& [h, info] : it->second)
				{
					auto res_type = info.resource->type();
					if (hash == h && res_type.is_derived_from(type))
					{
						UC_LOG_DEBUG(_logger) << "Get from cache " << res_type
							<< FromPath(path) << WithOptions(options);
						return info.resource;
					}
				}
			}

			if (auto resource = loader->load({ *this, path, options, logger }))
			{
				UC_LOG_DEBUG(_logger) << "Loaded " << resource->type()
					<< FromPath(path) << WithOptions(options) << " by " << loader->type()
					<< " " << MemorySize{ resource->get_system_memory_use() };

				_resources.push_back(resource);
				if (resource->cache_policy() == ResourceCachePolicy::CanCache)
				{
					CachedInfo info{ resource, path, loader.get() };
					_cached[loader.get()].emplace(hash, info);

					UC_LOG_DEBUG(_logger) << "Added " << resource->type()
						<< FromPath(path) << WithOptions(options);
				}

				return resource;
			}

			//UC_LOG_ERROR(logger) << "Failed to load " << type << " with loader";
		}

		UC_LOG_ERROR(logger) << "Failed to load "
			<< type << FromPath(path) << WithOptions(options);
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
			for (const auto& jt : it.second)
			{
				const auto& info = jt.second;
				if (system != nullptr)
					*system += info.resource->get_system_memory_use();
				if (video != nullptr)
				{
					if (const auto render_resource = std::dynamic_pointer_cast<RendererResource>(info.resource))
						*video += render_resource->get_video_memory_use();
				}
			}
		}
	}

	void ResourceCache::add_loader(const Shared<ResourceLoader>& loader)
	{
		for (const auto& resource_type : loader->resource_types())
			_loaders[resource_type].insert(loader);
		UC_LOG_DEBUG(_logger) << "Add " << loader->type();
	}

	void ResourceCache::unregister_module(const ModuleContext& context)
	{
		Module::unregister_module(context);

		unload_all();

		_loaders.clear();
	}

	// ============================================================================
	bool ResourceCache::LoaderSort::operator()(
		const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const
	{
		if (lhs->priority() == rhs->priority())
			return lhs < rhs;
		return lhs->priority() < rhs->priority();
	}

	size_t ResourceCache::make_hash(const Path& path, const ResourceOptions* options)
	{
		return options
			? Math::hash(path, options->hash())
			: Math::hash(path);
	}
}