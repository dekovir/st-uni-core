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

	namespace linq
	{
		template<typename T, typename TIterator>
		class IteratorRange
		{
		public:
			constexpr IteratorRange(TIterator begin, TIterator end)
				: _begin(begin), _end(end)
			{}

			UC_NODISCARD bool any(const Predicate<T>& predicate) const
			{
				return std::any_of(_begin, _end, predicate);
			}

			UC_NODISCARD bool all(const Predicate<T>& predicate) const
			{
				return std::all_of(_begin, _end, predicate);
			}

			UC_NODISCARD bool none(const Predicate<T>& predicate) const
			{
				return std::none_of(_begin, _end, predicate);
			}

			UC_NODISCARD TIterator find(const Predicate<T>& predicate) const
			{
				return std::find_if(_begin, _end, predicate);
			}

		protected:
			TIterator _begin, _end;
		};

		template<typename T>
		static auto from(const List<T>& list)
		{
			return IteratorRange<T, typename List<T>::const_iterator>(list.begin(), list.end());
		}

		template<typename T>
		static auto from_reverse(const List<T>& list)
		{
			return IteratorRange<T, typename List<T>::const_iterator>(list.rbegin(), list.rend());
		}
	}

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
		const auto range = _cached.equal_range(hash);
		for (auto it = range.first; it != range.second; ++it)
		{
			auto& info = it->second;
			auto res_type = info.resource->type();
			if (res_type.is_derived_from(type))
				return info.resource;
		}

		return nullptr;
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

	Shared<ReadFile> ResourceCache::open_read(const Path& path, bool quiet)
	{
		auto file = _provider.open_read(path);
		if (!file)
		{
			if (!quiet)
				UC_LOG_ERROR(_logger) << "Failed to open_read" << path;
			return nullptr;
		}

		return file;
	}

	Shared<Resource> ResourceCache::create(TypeConstRef type, const ResourceOptions& options)
	{
		return load(Path::Empty, type, &options, ResourceCacheFlags::Zero);
	}

	Shared<Resource> ResourceCache::load(const Path& path,
		TypeConstRef type, const ResourceOptions* options, ResourceCacheFlags flags)
	{
		if (auto resource_find = find(path, type, options))
			return resource_find;

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
				_resources.push_back(resource);
				add_resource(resource, path, options);

				return resource;
			}

			UC_LOG_ERROR(logger) << "Failed to load " << type << " with loader";
		}

		UC_LOG_ERROR(logger) << "Failed to load " << type << " from " << path;
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
	bool ResourceCache::LoaderSort::operator()(const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const
	{
		if (lhs->priority() == rhs->priority())
			return lhs < rhs;
		return lhs->priority() < rhs->priority();
	}

	bool ResourceCache::add_resource(const Shared<Resource>& resource,
		const Path& path, const ResourceOptions* options)
	{
		if (resource->cache_policy() == ResourceCachePolicy::CanCache)
		{
			const auto hash = make_hash(path, options);
			const auto& type = resource->type();

			CachedInfo info{ resource, path };
			_cached.emplace(hash, info);
			UC_LOG_DEBUG(_logger) << "Added " << type << " from " << path
				<< " " << MemorySize{ resource->get_system_memory_use() };
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