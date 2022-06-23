#pragma once
#include "unicore/Module.hpp"
#include "unicore/Logger.hpp"
#include "unicore/StreamProvider.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class ResourceCache : public Module
	{
	public:
		explicit ResourceCache(const Shared<Logger>& logger) : _logger(logger) {}

		void add_provider(const Shared<BasicStreamProvider>& provider);

		void add_loader(const Shared<ResourceLoader>& loader);
		UC_NODISCARD const List<Shared<ResourceLoader>>& get_loaders(TypeIndex index) const;

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

	protected:
		Shared<Logger> _logger;
		List<Shared<BasicStreamProvider>> _providers;
		Dictionary<TypeIndex, List<Shared<ResourceLoader>>> _loaders_dict;

		Dictionary<Path, Dictionary<TypeIndex, Shared<Resource>>> _cached;
	};
}