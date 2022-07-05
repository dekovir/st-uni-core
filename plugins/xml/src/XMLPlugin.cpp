#include "unicore/xml/XMLPlugin.hpp"
#include "XMLLoader.hpp"

namespace unicore
{
	static XMLDataLoader xml_loader;

	void XMLPlugin::register_module(Context& context)
	{
		Plugin::register_module(context);
		context.add_loader(xml_loader);
	}

	void XMLPlugin::unregister_module(Context& context)
	{
		Plugin::unregister_module(context);
		context.remove_loader(xml_loader);
	}
}