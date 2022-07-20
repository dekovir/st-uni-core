#include "SDL2Display.hpp"

#include "SDL2Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL_syswm.h>

namespace unicore
{
	SDL2Display::SDL2Display(SDL2Platform& platform, const DisplaySettings& settings)
		: ParentType(settings)
		, _platform(platform)
		, _handle(nullptr)
		, _logger(settings.logger)
	{
		SDL_Init(SDL_INIT_VIDEO);

		const auto flags = SDL_WINDOW_OPENGL | convert_flags(settings.flags);
		_handle = SDL_CreateWindow(settings.title.data(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			settings.size.x, settings.size.y, flags);

		if (!_handle)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return;
		}

		update_size();

		_platform.add_listener(this);

		//platform.on_sdl_event() += [&](const SDL_Event& evt)
		//{
		//	if (evt.type == SDL_WINDOWEVENT)
		//		apply(evt.window);
		//};
	}

	SDL2Display::~SDL2Display()
	{
		_platform.remove_listener(this);
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

	bool SDL2Display::on_event(const SDL_Event& evt)
	{
		if (evt.type == SDL_WINDOWEVENT)
		{
			switch (evt.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				update_size();
				UC_LOG_DEBUG(_logger) << "Resized to " << _size;
				_event_resize(_size);
				break;
			}

			return true;
		}

		return false;
	}

	// PROTECTED /////////////////////////////////////////////////////////////////
	void SDL2Display::update_size()
	{
		SDL_GetWindowSize(_handle, &_size.x, &_size.y);
	}

	Uint32 SDL2Display::convert_flags(DisplayFlags flags)
	{
		Uint32 value = 0;
#if !defined(UNICORE_PLATFORM_WEB)
		if (flags.has(DisplayFlag::Resizable))
			value |= SDL_WINDOW_RESIZABLE;

		if (flags.has(DisplayFlag::Borderless))
			value |= SDL_WINDOW_BORDERLESS;

		if (flags.has(DisplayFlag::Fullscreen))
			value |= SDL_WINDOW_FULLSCREEN;
#endif

		return value;
	}
}
#endif