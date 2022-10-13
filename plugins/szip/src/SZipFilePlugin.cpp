#include "unicore/szip/SZipFilePlugin.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/szip/SZipFileProviderLoader.hpp"

namespace unicore
{
	void SZipFilePlugin::register_plugin(const ModuleContext& context)
	{
		Plugin::register_plugin(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<SZipFileProviderLoader>());
		}
	}
}