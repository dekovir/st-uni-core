#include "TTFontLoader.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TTFontLoader::TTFontLoader(Renderer& renderer)
		: ResourceLoaderT({ L".ttf" })
		, _renderer(renderer)
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

		if (const auto data = dynamic_cast<const TTFontOptions*>(context.options))
			return factory->create(_renderer, *data, context.logger);

		const TTFontOptions params;
		return factory->create(_renderer, params, context.logger);
	}
}