#include "unicore/plugins/PlatformPlugin.hpp"
#include "unicore/ResourceCache.hpp"
#include "Resources/BinaryDataLoader.hpp"
#include "Resources/TextDataLoader.hpp"

namespace unicore
{
	void PlatformPlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<BinaryDataLoader>());
			cache->add_loader(std::make_shared<TextDataLoader>());
			cache->add_loader(std::make_shared<WTextDataLoader>());
		}
	}
}