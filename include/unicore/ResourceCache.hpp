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
	class ResourceConverter;
	class ResourceCreator;

	class ReadFile;
	class ReadFileProvider;

	enum class ResourceCacheFlag
	{
		Quiet = 1 << 0,
		IgnoreExtension = 1 << 1,
	};
	UNICORE_ENUM_FLAGS(ResourceCacheFlag, ResourceCacheFlags);

	class ResourceCache : public Module
	{
		UC_OBJECT(ResourceCache, Module)
	public:
		ResourceCache(Logger& logger, ReadFileProvider& provider);

		void unload_all();
		void unload_unused();

		UC_NODISCARD Shared<Resource> find(const Path& path,
			TypeConstRef type, const ResourceOptions* options = nullptr) const;
		UC_NODISCARD Optional<Path> find_path(const Resource& resource) const;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD Shared<T> find(const Path& path) const
		{
			return std::dynamic_pointer_cast<T>(find(path, get_type<T>()));
		}

		Shared<Resource> create(TypeConstRef type, const Any& value);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> create(const std::any& value)
		{
			return std::dynamic_pointer_cast<T>(create(get_type<T>(), value));
		}

		Shared<Resource> load(const Path& path, TypeConstRef type,
			const ResourceOptions* options, ResourceCacheFlags flags);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type<T>(), nullptr, flags));
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
		void add_converter(const Shared<ResourceConverter>& converter);
		void add_creator(const Shared<ResourceCreator>& creator);

		void unregister_module(const ModuleContext& context) override;

	protected:
		struct LoaderSort
		{
			bool operator()(const Shared<ResourceLoader>& lhs, const Shared<ResourceLoader>& rhs) const;
		};

		struct CreatorSort
		{
			bool operator()(const Shared<ResourceCreator>& lhs, const Shared<ResourceCreator>& rhs) const;
		};

		Logger& _logger;
		ReadFileProvider& _provider;

		Dictionary<TypeConstPtr, Set<Shared<ResourceLoader>, LoaderSort>> _loaders;
		Dictionary<TypeConstPtr, Set<Shared<ResourceConverter>>> _converters;
		Dictionary<TypeConstPtr, Set<Shared<ResourceCreator>, CreatorSort>> _creators;

		struct CachedInfo
		{
			Shared<Resource> resource;
			Path path;
			//Shared<ResourceOptions> options;
		};

		List<Weak<Resource>> _resources;
		DictionaryMulti<size_t, CachedInfo> _cached;

		Shared<Resource> load_resource(const Path& path, TypeConstRef type,
			const ResourceOptions* options, ResourceCacheFlags flags, Logger* logger);

		Shared<Resource> load_resource(ResourceLoader& loader, const Path& path,
			ReadFile& file, const ResourceOptions* options, Logger* logger);

		bool add_resource(const Shared<Resource>& resource,
			const Path& path, const ResourceOptions* options);

		static size_t make_hash(const Path& path, const ResourceOptions* options);
	};
}