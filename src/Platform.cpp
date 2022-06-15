#include "unicore/Platform.hpp"
#include "SDL2/SDL2Platform.hpp"

namespace unicore
{
	Platform::Platform(const PlatformSettings& settings)
		: _system_logger(Logger::create())
		, logger(*_system_logger)
		, time(settings.time)
		, input(settings.input)
		, render(settings.render)
	{
	}

	Shared<Platform> Platform::create()
	{
#if defined(UNICORE_USE_SDL2)
		unicore::SDLPlatformSettings settings;
#if defined(UNICORE_PLATFORM_EMSCRIPTEN)
#	if 0
		emscripten_get_screen_size(
			&settings.render.size.x,
			&settings.render.size.y);
#	else
		settings.render.size.x = 800;
		settings.render.size.y = 600;
#	endif
#else
		settings.render.size = unicore::Vector2i::Zero;
		//settings.render.size = unicore::Vector2i(800, 600);
		settings.render.resizeable = false;
		settings.render.borderless = false;
		settings.render.fullscreen = false;
#endif

		return make_shared<SDL2Platform>(settings);
#else
		static_assert(true, "Unknown platform");
#endif
	}
}