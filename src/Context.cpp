#include "unicore/Context.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	void Context::add_loader(ResourceLoader& loader)
	{
		_loaders[loader.resource_type()].insert(&loader);
	}

	const Set<ResourceLoader*>& Context::get_loaders(TypeIndex type) const
	{
		static Set<ResourceLoader*> s_empty;
		const auto it = _loaders.find(type);
		return it != _loaders.end() ? it->second : s_empty;
	}
}