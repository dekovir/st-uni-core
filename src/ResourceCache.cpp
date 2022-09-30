#include "unicore/ResourceCache.hpp"
#include "unicore/Math.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/FileProvider.hpp"
#include "unicore/RendererResource.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	static auto& s_resource_type = get_type<Resource>();

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
		for (auto it = _cached.begin(); it != _cached.end();)
		{
			auto& info = it->second;
			if (info.resource.use_count() == 1)
			{
				UC_LOG_DEBUG(_logger) << "Unload resource "
					<< info.resource->type() << " from " << info.path;
				it = _cached.erase(it);
			}
			else ++it;
		}

		// Remove expired weak pointers
		for (auto it = _resources.begin(); it != _resources.end();)
		{
			if (it->use_count() > 0) ++it;
			else it = _resources.erase(it);
		}
	}

	Shared<Resource> ResourceCache::find(const Path& path,
		TypeConstRef type, const ResourceOptions* options) const
	{
		const size_t hash = make_hash(path, options);
		const auto info = internal_find(type, hash);
		return info ? info->resource : nullptr;
	}

	Optional<Path> ResourceCache::find_path(const Resource& resource) const
	{
		for (const auto& it : _cached)
		{
			auto& info = it.second;
			if (info.resource.get() == &resource)
				return info.path;
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
		const size_t hash = make_hash(path, options);
		if (const auto cached_info = internal_find(type, hash))
			return cached_info->resource;

		const auto logger = !flags.has(ResourceCacheFlag::Quiet) ? &_logger : nullptr;

		const auto it = _loaders.find(&type);
		if (it == _loaders.end())
		{
			UC_LOG_ERROR(logger) << "No loaders for " << type;
			return nullptr;
		}

		// TODO: Implement loading stack for prevent recursive loading
		const auto& loaders = it->second;
		const auto extension = path.extension();
		for (const auto& loader : loaders)
		{
			if (!flags.has(ResourceCacheFlag::IgnoreExtension) && !loader->can_load(path))
				continue;

			if (!loader->can_load(options))
				continue;

			if (auto resource = loader->load({ *this, path, options, logger }))
			{
				UC_LOG_DEBUG(_logger) << "Loaded " << resource->type()
					<< FromPath(path) << WithOptions(options)
					<< " " << MemorySize{ resource->get_system_memory_use() };

				_resources.push_back(resource);
				internal_add(resource, path, options, hash);

				return resource;
			}

			UC_LOG_ERROR(logger) << "Failed to load " << type << " with loader";
		}

		UC_LOG_ERROR(logger) << "Failed to load " << type << FromPath(path) << WithOptions(options);
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
			const auto& info = it.second;
			if (system != nullptr)
				*system += info.resource->get_system_memory_use();
			if (video != nullptr)
			{
				if (const auto render_resource = std::dynamic_pointer_cast<RendererResource>(info.resource))
					*video += render_resource->get_video_memory_use();
			}
		}
	}

	void ResourceCache::add_loader(const Shared<ResourceLoader>& loader)
	{
		for (auto type = &loader->resource_type(); type != nullptr; type = type->parent)
		{
			_loaders[type].insert(loader);
			if (type == &s_resource_type)
				break;
		}
	}

	void ResourceCache::unregister_module(const ModuleContext& context)
	{
		Module::unregister_module(context);

		_loaders.clear();

		unload_all();
	}

	// ============================================================================
	bool ResourceCache::LoaderSort::operator()(
		const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const
	{
		if (lhs->priority() == rhs->priority())
			return lhs < rhs;
		return lhs->priority() < rhs->priority();
	}

	const ResourceCache::CachedInfo* ResourceCache::internal_find(TypeConstRef type, size_t hash) const
	{
		const auto range = _cached.equal_range(hash);
		for (auto it = range.first; it != range.second; ++it)
		{
			auto& info = it->second;
			auto res_type = info.resource->type();
			if (res_type.is_derived_from(type))
				return &info;
		}

		return nullptr;
	}

	bool ResourceCache::internal_add(const Shared<Resource>& resource,
		const Path& path, const ResourceOptions* options, size_t hash)
	{
		if (resource->cache_policy() == ResourceCachePolicy::CanCache)
		{
			CachedInfo info{ resource, path };
			_cached.emplace(hash, info);

			UC_LOG_DEBUG(_logger) << "Added " << resource->type()
				<< FromPath(path) << WithOptions(options);

			return true;
		}

		return false;
	}

	size_t ResourceCache::make_hash(const Path& path, const ResourceOptions* options)
	{
		return options
			? Math::hash(path, options->hash())
			: Math::hash(path);
	}
}