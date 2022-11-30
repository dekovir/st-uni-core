#include "unicore/remoteui/RemoteUIPlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/remoteui/DocumentXMLLoader.hpp"

namespace unicore::remoteui
{
	void RemoteUIPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_XML)
			cache->add_loader(std::make_shared<DocumentXMLLoader>());
#endif
		}
	}
}