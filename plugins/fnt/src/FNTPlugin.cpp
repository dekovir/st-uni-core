#include "unicore/fnt/FNTPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "BitmapFontLoader.hpp"

namespace unicore
{
	void FNTPlugin::register_module(const ModuleContext& context)
	{
		Plugin::register_module(context);
		if (const auto cache = context.modules.find<ResourceCache>())
			cache->add_loader(std::make_shared<BitmapFontLoader>());
	}
}