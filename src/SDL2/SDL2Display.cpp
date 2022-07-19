#include "SDL2Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL_syswm.h>

namespace unicore
{
	SDL2Display::SDL2Display(const DisplaySettings& settings)
		: ParentType(settings)
		, _handle(nullptr)
		, _logger(settings.logger)
	{
		SDL_Init(SDL_INIT_VIDEO);

		const auto flags = SDL_WINDOW_OPENGL | convert_flags(settings.flags);
		_handle = SDL_CreateWindow(settings.title.data(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			settings.size.x, settings.size.y, flags);

		update_size();

		SDL_DisableScreenSaver();
	}

	SDL2Display::~SDL2Display()
	{
		SDL_DestroyWindow(_handle);
	}

	void* SDL2Display::native_handle() const
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

	void SDL2Display::apply(const SDL_WindowEvent& evt)
	{
		switch (evt.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			update_size();
			_event_resize(_size);
			break;
		}
	}

	// PROTECTED /////////////////////////////////////////////////////////////////
	void SDL2Display::update_size()
	{
		SDL_GetWindowSize(_handle, &_size.x, &_size.y);
	}

	Uint32 SDL2Display::convert_flags(DisplayFlags flags)
	{
		Uint32 value = 0;

		if (flags.has(DisplayFlag::Resizable))
			value |= SDL_WINDOW_RESIZABLE;

		if (flags.has(DisplayFlag::Borderless))
			value |= SDL_WINDOW_BORDERLESS;

		if (flags.has(DisplayFlag::Fullscreen))
			value |= SDL_WINDOW_FULLSCREEN;

		return value;
	}
}
#endif