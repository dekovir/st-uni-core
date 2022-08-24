#include "unicore/RendererSDL.hpp"
#include "Platforms/SDL2/SDL2RendererSDL.hpp"

namespace unicore
{
	RendererSDL::RendererSDL(Logger& logger)
		: ParentType(logger)
	{
	}

	Unique<RendererSDL> RendererSDL::create(Logger& logger, Display& display)
	{
#if defined(UNICORE_USE_SDL2)
		return SDL2RendererSDL::create(logger, display);
#else
		UC_STATIC_ASSERT_ALWAYS("Unknown platform");
#endif
	}
}