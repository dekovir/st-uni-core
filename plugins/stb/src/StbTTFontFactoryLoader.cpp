#include "StbTTFontFactoryLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
namespace unicore
{
	StbTTFontFactoryLoader::StbTTFontFactoryLoader(Renderer& renderer)
		: ResourceLoaderT({ L".ttf" })
		, _renderer(renderer)
	{
	}

	Shared<Resource> StbTTFontFactoryLoader::load(const Context& options)
	{
		auto data = options.file.as_data();
		auto factory = std::make_shared<StbTTFontFactory>(_renderer, data);
		if (!factory->valid())
		{
			UC_LOG_ERROR(options.logger) << "Failed to load font from" << options.path;
			return nullptr;
		}

		return factory;
	}
}
#endif