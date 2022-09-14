#include "unicore/plugins/SurfacePlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "Resources/SolidSizeCreator.hpp"
#include "Resources/SizeResourceCreator.hpp"
#include "Resources/SpriteListTileSetCreator.hpp"

namespace unicore
{
	void SurfacePlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_creator(std::make_shared<SurfaceSizeSurfaceCreator>());
			cache->add_creator(std::make_shared<DynamicSurfaceSolidSizeCreator>());

			cache->add_creator(std::make_shared<DynamicSurfaceSizeCreator>());

			cache->add_creator(std::make_shared<SpriteListTileSetCreator>());
		}
	}
}
