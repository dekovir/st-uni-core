#include "unicore/plugins/RendererPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Renderer.hpp"
#include "Resources/TextureConverter.hpp"
#include "Resources/SolidSizeCreator.hpp"
#include "Resources/SizeResourceCreator.hpp"

namespace unicore
{
	void RendererPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (auto [cache, renderer] = context.modules.finds<ResourceCache, Renderer>(); cache && renderer)
		{
			cache->add_converter(std::make_shared<TextureConverter>(*renderer));
			cache->add_converter(std::make_shared<DynamicTextureConverter>(*renderer));

			cache->add_creator(std::make_shared<TextureSolidSizeCreator>(*renderer));
			cache->add_creator(std::make_shared<DynamicTextureSizeCreator>(*renderer));
			cache->add_creator(std::make_shared<TargetTextureSizeCreator>(*renderer));
		}
	}
}