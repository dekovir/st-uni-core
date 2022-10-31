#include "unicore/ui/UIPlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/ui/UIDocumentXMLLoader.hpp"

namespace unicore
{
	void UIPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_XML)
			cache->add_loader(std::make_shared<UIDocumentXMLLoader>());
#endif
		}
	}
}