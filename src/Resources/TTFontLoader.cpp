#include "TTFontLoader.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TTFontLoader::TTFontLoader()
		: ResourceLoaderT({ L".ttf" })
	{
	}

	Shared<Resource> TTFontLoader::load(const Context& context)
	{
		const auto factory = context.cache.load<TTFontFactory>(
			context.path, ResourceCacheFlag::IgnoreExtension);
		if (!factory)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load font";
			return nullptr;
		}

		if (const auto options = dynamic_cast<const TTFontOptions*>(context.options))
			return factory->create_options({ context.options, context.logger }, *options);

		const TTFontOptions params;
		return factory->create_default({ nullptr, context.logger });
	}
}