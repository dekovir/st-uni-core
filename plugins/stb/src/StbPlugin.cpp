#include "unicore/stb/StbPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "STBSurfaceLoader.hpp"

namespace unicore
{
	void StbPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<STBSurfaceLoader>());
			cache->add_loader(std::make_shared<STBDynamicSurfaceLoader>());
		}
	}
}