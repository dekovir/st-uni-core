#include "unicore/plugins/RendererPlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/renderer/TextureLoader.hpp"
#include "unicore/renderer/SolidSizeLoader.hpp"
#include "unicore/renderer/TTFontLoader.hpp"
#include "unicore/renderer/Renderer.hpp"

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