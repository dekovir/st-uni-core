#include "SDL2Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL_syswm.h>

namespace unicore
{
	SDL2Display::SDL2Display(SDL_Window* handle, Logger& logger)
		: _handle(handle), _logger(logger)
	{
		update_size();

		SDL_DisableScreenSaver();
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

	Shared<SDL2Display> SDL2Display::create(const Settings& settings)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			UC_LOG_ERROR(settings.logger) << SDL_GetError();
			return nullptr;
		}

		const auto flags = SDL_WINDOW_OPENGL | convert_flags(settings.flags);
		auto handle = SDL_CreateWindow(settings.title.data(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			settings.size.x, settings.size.y, flags);

		if (!handle)
		{
			UC_LOG_ERROR(settings.logger) << SDL_GetError();
			return nullptr;
		}

		return std::make_shared<SDL2Display>(handle, settings.logger);
	}

	// PROTECTED /////////////////////////////////////////////////////////////////
	void SDL2Display::update_size()
	{
		SDL_GetWindowSize(_handle, &_size.x, &_size.y);
	}

	Uint32 SDL2Display::convert_flags(SDL2DisplayFlags flags)
	{
		Uint32 value = 0;

		if (flags.has(SDL2DisplayFlag::Resizable))
			value |= SDL_WINDOW_RESIZABLE;

		if (flags.has(SDL2DisplayFlag::Borderless))
			value |= SDL_WINDOW_BORDERLESS;

		if (flags.has(SDL2DisplayFlag::Fullscreen))
			value |= SDL_WINDOW_FULLSCREEN;

		return value;
	}
}
#endif