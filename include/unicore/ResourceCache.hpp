#pragma once
#include "unicore/Module.hpp"
#include "unicore/StreamProvider.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class Logger;
	class Context;
	class ResourceLoader;
	class ResourceConverter;
	class ResourceCreator;

	enum class ResourceCacheFlag
	{
		Quiet = 1,
	};
	UNICORE_ENUMFLAGS(ResourceCacheFlag, ResourceCacheFlags);

	class ResourceCache : public Module
	{
		UC_OBJECT(ResourceCache, Module)
	public:
		explicit ResourceCache(Logger& logger);

		void unload_all();
		void unload_unused();

		void clear();

		void add_provider(StreamProvider& provider);

		UC_NODISCARD Shared<ReadStream> open_read(const Path& path) const;

		UC_NODISCARD Shared<Resource> find(const Path& path, TypeConstRef type) const;
		UC_NODISCARD Optional<Path> find_path(const Resource& resource) const;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD Shared<T> find(const Path& path) const
		{
			return std::dynamic_pointer_cast<T>(find(path, get_type<T>()));
		}

		Shared<Resource> create(TypeConstRef type, const std::any& value);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> create(const std::any& value)
		{
			return std::dynamic_pointer_cast<T>(create(get_type<T>(), value));
		}

		Shared<Resource> load(const Path& path, TypeConstRef type,
			ResourceCacheFlags flags = ResourceCacheFlags::Zero);

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type<T>(), flags));
		}

		void dump_used();
		void calc_memory_use(size_t* system, size_t* video) const;

		void add_loader(const Shared<ResourceLoader>& loader);
		void add_converter(const Shared<ResourceConverter>& converter);
		void add_creator(const Shared<ResourceCreator>& creator);

		void register_module(const ModuleContext& context) override;
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
		List<StreamProvider*> _providers;

		Dictionary<TypeConstPtr, Set<Shared<ResourceLoader>, LoaderSort>> _loaders;
		Dictionary<TypeConstPtr, Set<Shared<ResourceConverter>>> _converters;
		Dictionary<TypeConstPtr, Set<Shared<ResourceCreator>, CreatorSort>> _creators;

		List<Weak<Resource>> _resources;
		Dictionary<Path, Dictionary<TypeConstPtr, Shared<Resource>>> _cached;

		Shared<Resource> load_resource(const Path& path, TypeConstRef type, Logger* logger);

		Shared<Resource> load_resource(ResourceLoader& loader,
			const Path& path, ReadStream& stream, TypeConstRef type, Logger* logger);

		bool add_resource(const Shared<Resource>& resource, const Path& path, TypeConstRef type);
	};
}