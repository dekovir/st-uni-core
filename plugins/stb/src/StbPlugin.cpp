#include "unicore/stb/StbPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "EasyFontCreator.hpp"
#include "SurfaceLoader.hpp"

namespace unicore
{
	void StbPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_STB_EASY_FONT)
			cache->add_creator(std::make_shared<EasyFontCreator>());
#endif

#if defined(UNICORE_USE_STB_IMAGE)
			cache->add_loader(std::make_shared<StbSurfaceLoader>());
			cache->add_loader(std::make_shared<STBDynamicSurfaceLoader>());
#endif
		}
	}
}