#include "unicore/plugins/SurfacePlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "Resources/STBSurfaceLoader.hpp"
#include "Resources/SolidSurfaceCreator.hpp"
#include "Resources/SizeResourceCreator.hpp"

namespace unicore
{
	void SurfacePlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_STB)
			cache->add_loader(std::make_shared<STBSurfaceLoader>());
			cache->add_loader(std::make_shared<STBDynamicSurfaceLoader>());
#endif
			cache->add_creator(std::make_shared<SurfaceSolidSurfaceCreator>());
			cache->add_creator(std::make_shared<DynamicSurfaceSolidSurfaceCreator>());

			cache->add_creator(std::make_shared<DynamicSurfaceSizeCreator>());
		}
	}
}