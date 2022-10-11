#pragma once
#include "unicore/Module.hpp"
#include "unicore/EnumFlag.hpp"
#include "unicore/Path.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class Logger;
	class Context;
	class ResourceLoader;

	enum class ResourceCacheFlag
	{
		Quiet = 1 << 0,
	};
	UNICORE_ENUM_FLAGS(ResourceCacheFlag, ResourceCacheFlags);

	class IResourceCache
	{
	public:
		virtual ~IResourceCache() = default;

		// CREATE ////////////////////////////////////////////////////////////////////
		Shared<Resource> create(TypeConstRef type, const ResourceOptions& options)
		{
			return load_raw(Path::Empty, type, &options, ResourceCacheFlags::Zero);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> create(const ResourceOptions& options)
		{
			auto& type = get_type<T>();
			auto resource = create(type, options);
			return std::dynamic_pointer_cast<T>(resource);
		}

		// LOAD //////////////////////////////////////////////////////////////////////
		virtual Shared<Resource> load_raw(const Path& path, TypeConstRef type,
			const ResourceOptions* options, ResourceCacheFlags flags) = 0;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			auto& type = get_type<T>();
			auto resource = load_raw(path, type, nullptr, flags);
			return std::dynamic_pointer_cast<T>(resource);
		}

		template<typename T, typename TData,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr,
			std::enable_if_t<std::is_base_of_v<ResourceOptions, TData>>* = nullptr>
		Shared<T> load(const Path& path, const TData& options, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			auto& type = get_type<T>();
			auto resource = load_raw(path, type, &options, flags);
			return std::dynamic_pointer_cast<T>(resource);
		}
	};

	class ResourceCache : public Module, public IResourceCache
	{
		UC_OBJECT(ResourceCache, Module)
	public:
		explicit ResourceCache(Logger& logger);

		void unload_all();
		void unload_unused();

		UC_NODISCARD Optional<Path> find_path(const Resource& resource) const;

		Shared<Resource> load_raw(const Path& path, TypeConstRef type,
			const ResourceOptions* options, ResourceCacheFlags flags) override;

		void dump_used();
		void calc_memory_use(size_t* system, size_t* video) const;

		void add_loader(const Shared<ResourceLoader>& loader);

		void unregister_module(const ModuleContext& context) override;

	protected:
		struct LoaderSort
		{
			bool operator()(const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const;
		};

		Logger& _logger;
		Dictionary<TypeConstPtr, Set<Shared<ResourceLoader>, LoaderSort>> _loaders;

		struct CachedInfo
		{
			Shared<Resource> resource;
			Path path;
			ResourceLoader* loader;
			//? Shared<ResourceOptions> options;
		};

		List<Weak<Resource>> _resources;
		Dictionary<ResourceLoader*, Dictionary<size_t, CachedInfo>> _cached;

		static size_t make_hash(const Path& path, const ResourceOptions* options);
	};
}