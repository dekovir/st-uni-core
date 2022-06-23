#pragma once
#include "unicore/Logger.hpp"
#include "unicore/StreamProvider.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class Context;

	class ResourceCache : public Object
	{
	public:
		explicit ResourceCache(Context& context, Logger& logger);

		void unload_all();
		void clear();

		void add_provider(BasicStreamProvider& provider);

		UC_NODISCARD Shared<Resource> find(const Path& path, TypeIndex type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD Shared<T> find(const Path& path) const
		{
			return std::dynamic_pointer_cast<T>(find(path, get_type_index<T>()));
		}

		Shared<Resource> load(const Path& path, TypeIndex type);

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		Shared<T> load(const Path& path)
		{
			return std::dynamic_pointer_cast<T>(load(path, get_type_index<T>()));
		}

		void calc_memory_use(size_t* system, size_t* video) const;

	protected:
		Context& _context;
		Logger& _logger;
		List<BasicStreamProvider*> _providers;

		Dictionary<Path, Dictionary<TypeIndex, Shared<Resource>>> _cached;
	};
}