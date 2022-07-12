#include "SDL2Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL_syswm.h>

namespace unicore
{
	SDL2Display::SDL2Display(SDL_Window* handle, Logger& logger)
		: _handle(handle), _logger(logger)
	{
		update_size();
	}

	void* SDL2Display::handle() const
	{
		SDL_SysWMinfo wmi;
		if (!SDL_GetWindowWMInfo(_handle, &wmi))
			return nullptr;

#if defined(SDL_VIDEO_DRIVER_WINDOWS)
		return wmi.info.win.window;
#elif defined(SDL_VIDEO_DRIVER_X11)
		return (void*)wmi.info.x11.window;
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
		return wmi.android.window;
#else
		//UC_STATIC_ASSERT_ALWAYS("Invalid platform");
		return nullptr;
#endif
	}

	void SDL2Display::update_size()
	{
		SDL_GetWindowSize(_handle, &_size.x, &_size.y);
	}
}
#endif