#include "StbTTFontFactoryLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	StbTTFontFactoryLoader::StbTTFontFactoryLoader(Renderer& renderer)
		: _renderer(renderer)
	{
	}

	Shared<Resource> StbTTFontFactoryLoader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.load<ReadFile>(context.path);
		if (!file) return nullptr;

		auto data = file->as_data();
		auto factory = std::make_shared<StbTTFontFactory>(_renderer, data);
		if (!factory->valid())
		{
			UC_LOG_ERROR(context.logger) << "Failed to load font from" << context.path;
			return nullptr;
		}

		return factory;
	}
}
#endif