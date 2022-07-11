#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Logger;
	class Resource;
	class ResourceLoader;
	class ResourceConverter;

	class Context
	{
	public:
		void register_type(TypeConstRef type);
		void dump_types(Logger& logger) const;

		void add_loader(ResourceLoader& loader);
		void remove_loader(ResourceLoader& loader);

		UC_NODISCARD const Set<ResourceLoader*>& get_loaders(TypeConstRef type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD const Set<ResourceLoader*>& get_loaders() const
		{
			return get_loaders(get_type<T>());
		}

		void add_converter(ResourceConverter& converter);
		UC_NODISCARD const Set<ResourceConverter*>& get_converters(TypeConstRef type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD const Set<ResourceConverter*>& get_converters() const
		{
			return get_converters(get_type<T>());
		}

	protected:
		Set<TypeConstPtr> _types;
		Dictionary<TypeConstPtr, Set<TypeConstPtr>> _derived_types;

		Dictionary<TypeConstPtr, Set<ResourceLoader*>> _loaders;
		Dictionary<TypeConstPtr, Set<ResourceConverter*>> _converters;
	};
}