#include "unicore/stb/StbPlugin.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"
#include "StbEasyFontCreator.hpp"
#include "StbSurfaceLoader.hpp"
#include "StbTTFontFactoryLoader.hpp"

namespace unicore
{
	void StbPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_STB_EASY_FONT)
			cache->add_creator(std::make_shared<StbEasyFontCreator>());
#endif

#if defined(UNICORE_USE_STB_IMAGE)
			cache->add_loader(std::make_shared<StbSurfaceLoader>());
			cache->add_loader(std::make_shared<StbDynamicSurfaceLoader>());
#endif

			if (const auto renderer = context.modules.find<Renderer>())
			{
#if defined(UNICORE_USE_STB_TRUETYPE)
				cache->add_loader(std::make_shared<StbTTFontFactoryLoader>(*renderer));
#endif
			}
		}
	}
}