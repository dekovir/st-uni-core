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

	class ResourceCache : public Module
	{
		UC_OBJECT(ResourceCache, Module)
	public:
		explicit ResourceCache(Logger& logger);

		void unload_all();
		void unload_unused();

		UC_NODISCARD Optional<Path> find_path(const Resource& resource) const;

		// CREATE ////////////////////////////////////////////////////////////////////
		Shared<Resource> create(TypeConstRef type, const ResourceOptions& options);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> create(const ResourceOptions& options)
		{
			return std::dynamic_pointer_cast<T>(create(get_type<T>(), options));
		}

		// LOAD //////////////////////////////////////////////////////////////////////
		Shared<Resource> load(const Path& path, TypeConstRef type,
			const ResourceOptions* options, ResourceCacheFlags flags);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			auto& type = get_type<T>();
			auto resource = load(path, type, nullptr, flags);
			return std::dynamic_pointer_cast<T>(resource);
		}

		template<typename T, typename TData,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr,
			std::enable_if_t<std::is_base_of_v<ResourceOptions, TData>>* = nullptr>
		Shared<T> load(const Path& path, const TData& options, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type<T>(), &options, flags));
		}

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
			//? Shared<ResourceOptions> options;
		};

		List<Weak<Resource>> _resources;
		Dictionary<ResourceLoader*, Dictionary<size_t, CachedInfo>> _cached;

		static size_t make_hash(const Path& path, const ResourceOptions* options);
	};
}