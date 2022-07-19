#include "unicore/SDLRenderer.hpp"
#include "SDL2/RendererImpl.hpp"

namespace unicore
{
	SDLRenderer::SDLRenderer(Logger& logger)
		: ParentType(logger)
	{
	}

	Unique<SDLRenderer> SDLRenderer::create(Logger& logger, Display& display)
	{
#if defined(UNICORE_USE_SDL2)
		return RendererImpl::create(logger, display);
#else
		UC_STATIC_ASSERT_ALWAYS("Unknown platform");
#endif
	}
}