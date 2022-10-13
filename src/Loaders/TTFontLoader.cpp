#include "unicore/loaders/TTFontLoader.hpp"
#include "unicore/Memory.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/renderer/Renderer.hpp"

namespace unicore
{
	Shared<Resource> TTFontLoader::load_options(
		const Context& context, const TTFontOptions& options)
	{
		const auto factory = context.cache.load<TTFontFactory>(context.path);
		if (!factory)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load TTFont";
			return nullptr;
		}

		return factory->create(options, context.logger);
	}
}