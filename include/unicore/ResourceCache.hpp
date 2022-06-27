#pragma once
#include "unicore/Logger.hpp"
#include "unicore/StreamProvider.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class Context;

	enum class ResourceCacheFlag
	{
		Quiet = 1,
		SkipExtension = 2,
	};
	UNICORE_ENUMFLAGS(ResourceCacheFlag, ResourceCacheFlags);

	class ResourceCache : public Module
	{
	public:
		explicit ResourceCache(Logger& logger);

		void unload_all();
		void clear();

		void add_provider(StreamProvider& provider);

		UC_NODISCARD Shared<ReadStream> open_read(const Path& path) const;

		UC_NODISCARD Shared<Resource> find(const Path& path, TypeIndex type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD Shared<T> find(const Path& path) const
		{
			return std::dynamic_pointer_cast<T>(find(path, get_type_index<T>()));
		}

		Shared<Resource> load(const Path& path, TypeIndex type,
			ResourceCacheFlags flags = ResourceCacheFlags::Zero);

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path,
			ResourceCacheFlags flags = ResourceCacheFlags::Zero)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type_index<T>(), flags));
		}

		void calc_memory_use(size_t* system, size_t* video) const;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

	protected:
		Context* _context = nullptr;
		Logger& _logger;
		List<StreamProvider*> _providers;

		Dictionary<Path, Dictionary<TypeIndex, Shared<Resource>>> _cached;
	};
}