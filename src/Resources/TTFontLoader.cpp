#include "TTFontLoader.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TTFontLoader::TTFontLoader()
		: ResourceLoaderTypeOptions({ L".ttf" })
	{
	}

	Shared<Resource> TTFontLoader::load_options(const Context& context, const TTFontOptions& options)
	{
		const auto factory = context.cache.load<TTFontFactory>(
			context.path, ResourceCacheFlag::IgnoreExtension);
		if (!factory)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load font";
			return nullptr;
		}

		return factory->create_options({ nullptr, context.logger }, options);
	}
}