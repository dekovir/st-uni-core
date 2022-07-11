#pragma once
#include "unicore/StreamProvider.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class Logger;
	class Context;

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

		template<typename T>
		UC_NODISCARD Shared<T> find(const Path& path) const
		{
			return std::dynamic_pointer_cast<T>(find(path, get_type<T>()));
		}

		Shared<Resource> load(const Path& path, TypeConstRef type,
			ResourceCacheFlags flags = ResourceCacheFlags::Zero);

		template<typename T>
		Shared<T> load(const Path& path, ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type<T>(), flags));
		}

		void dump_used();
		void calc_memory_use(size_t* system, size_t* video) const;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

	protected:
		Context* _context = nullptr;
		Logger& _logger;
		List<StreamProvider*> _providers;

		Dictionary<Path, Dictionary<TypeConstPtr, Shared<Resource>>> _cached;

		Shared<Resource> load_resource(const Path& path, TypeConstRef type, Logger* logger);

		Shared<Resource> load_resource(ResourceLoader& loader,
			const Path& path, ReadStream& stream, TypeConstRef type, Logger* logger);

		bool add_resource(const Shared<Resource>& resource, const Path& path, TypeConstRef type);
	};
}