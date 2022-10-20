#include "unicore/resource/ResourcePlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/resource/BinaryDataLoader.hpp"
#include "unicore/resource/TextDataLoader.hpp"

namespace unicore
{
	void ResourcePlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<BinaryDataLoader>());
			cache->add_loader(std::make_shared<TextDataLoader>());
			//cache->add_loader(std::make_shared<TextData16Loader>());
			//cache->add_loader(std::make_shared<TextData32Loader>());
		}
	}
}