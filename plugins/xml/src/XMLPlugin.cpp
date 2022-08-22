#include "unicore/xml/XMLPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "XMLLoader.hpp"

namespace unicore
{
	static XMLDataLoader xml_loader;

	void XMLPlugin::register_module(const ModuleContext& context)
	{
		Plugin::register_module(context);
		if (const auto cache = context.modules.find<ResourceCache>())
			cache->add_loader(std::make_shared<XMLDataLoader>());
	}
}