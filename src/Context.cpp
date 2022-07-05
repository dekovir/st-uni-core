#include "unicore/Context.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	void Context::add_loader(ResourceLoader& loader)
	{
		_loaders[loader.resource_type()].insert(&loader);
	}

	void Context::remove_loader(ResourceLoader& loader)
	{
		if (const auto it = _loaders.find(loader.resource_type()); it != _loaders.end())
		{
			if (const auto jt = it->second.find(&loader); jt != it->second.end())
				it->second.erase(jt);
		}
	}

	const Set<ResourceLoader*>& Context::get_loaders(TypeIndex type) const
	{
		static Set<ResourceLoader*> s_empty;
		const auto it = _loaders.find(type);
		return it != _loaders.end() ? it->second : s_empty;
	}

	void Context::add_converter(ResourceConverter& converter)
	{
		_converters[converter.resource_type()].insert(&converter);
	}

	const Set<ResourceConverter*>& Context::get_converters(TypeIndex type) const
	{
		static Set<ResourceConverter*> s_empty;
		const auto it = _converters.find(type);
		return it != _converters.end() ? it->second : s_empty;
	}
}