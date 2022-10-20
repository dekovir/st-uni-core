#include "unicore/fnt/FNTPlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/fnt/DekovirFontLoader.hpp"

namespace unicore
{
	void FNTPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);
		if (const auto cache = context.modules.find<ResourceCache>())
			cache->add_loader(std::make_shared<DekovirFontLoader>());
	}
}