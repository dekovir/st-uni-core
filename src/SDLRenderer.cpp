#include "unicore/SDLRenderer.hpp"
#include "SDL2/RendererImpl.hpp"

namespace unicore
{
	SDLRenderer::SDLRenderer(Logger& logger)
		: ParentType(logger)
	{
	}

	Shared<SDLRenderer> SDLRenderer::create(Logger& logger)
	{
#if defined(UNICORE_USE_SDL2)
		SDL2RenderSettings settings;
		settings.size = VectorConst2i::Zero;
		//settings.size = Vector2i(800, 600);
		settings.resizeable = false;
		settings.borderless = false;
		settings.fullscreen = false;
		return make_unique<RendererImpl>(logger, settings);
#else
		UC_STATIC_ASSERT_ALWAYS("Unknown platform");
#endif
	}
}