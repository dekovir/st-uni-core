#include "unicore/plugins/RendererPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/loaders/TextureLoader.hpp"
#include "unicore/loaders/SolidSizeLoader.hpp"
#include "unicore/loaders/TTFontLoader.hpp"

namespace unicore
{
	void RendererPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>(); cache)
		{
			cache->add_loader(std::make_shared<TTFontLoader>());

			if (const auto renderer = context.modules.find<Renderer>(); renderer)
			{
				cache->add_loader(std::make_shared<TextureLoader>(*renderer));
				cache->add_loader(std::make_shared<DynamicTextureLoader>(*renderer));
				cache->add_loader(std::make_shared<TextureSolidSizeLoader>(*renderer));
			}
		}
	}
}