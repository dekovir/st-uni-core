#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Resource;
	class ResourceLoader;
	class ResourceConverter;

	class Context
	{
	public:
		void add_loader(ResourceLoader& loader);
		void remove_loader(ResourceLoader& loader);

		UC_NODISCARD const Set<ResourceLoader*>& get_loaders(TypeIndex type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD const Set<ResourceLoader*>& get_loaders() const
		{
			return get_loaders(get_type_index<T>());
		}

		void add_converter(ResourceConverter& converter);
		UC_NODISCARD const Set<ResourceConverter*>& get_converters(TypeIndex type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD const Set<ResourceConverter*>& get_converters() const
		{
			return get_converters(get_type_index<T>());
		}

	protected:
		Dictionary<TypeIndex, Set<ResourceLoader*>> _loaders;
		Dictionary<TypeIndex, Set<ResourceConverter*>> _converters;
	};
}