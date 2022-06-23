#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Resource;
	class ResourceLoader;

	class Context
	{
	public:
		void add_loader(ResourceLoader& loader);
		UC_NODISCARD const Set<ResourceLoader*>& get_loaders(TypeIndex type) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
		UC_NODISCARD const Set<ResourceLoader*>& get_loaders() const
		{
			return get_loaders(get_type_index<T>());
		}

	protected:
		Dictionary<TypeIndex, Set<ResourceLoader*>> _loaders;
	};
}