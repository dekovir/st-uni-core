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
	{
		SDL_Init(SDL_INIT_VIDEO);

		Vector2i size = settings.size;
		const auto flags = make_flags(size, settings.mode, settings.window_flags);

		_handle = SDL_CreateWindow(settings.title.data(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			size.x, size.y, flags);

		if (!_handle)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return;
		}

		update_size();

		_platform.add_listener(this);
	}

	SDL2Display::~SDL2Display()
	{
		_platform.remove_listener(this);
		SDL_DestroyWindow(_handle);
	}

	DisplayMode SDL2Display::mode() const
	{
		const auto flags = SDL_GetWindowFlags(_handle);
		return (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) || (flags & SDL_WINDOW_FULLSCREEN)
			? DisplayMode::Fullscreen : DisplayMode::Window;
	}

	DisplayWindowFlags SDL2Display::window_flags() const
	{
		auto flags = DisplayWindowFlags::Zero;
		const auto value = SDL_GetWindowFlags(_handle);

		if (value & SDL_WINDOW_RESIZABLE)
			flags |= DisplayWindowFlag::Resizable;
		if (value & SDL_WINDOW_BORDERLESS)
			flags |= DisplayWindowFlag::Borderless;

		return flags;
	}

	void SDL2Display::set_windowed(const Vector2i& size_, DisplayWindowFlags window_flags)
	{
		Vector2i size = size_;
		const auto flags = make_flags(size, DisplayMode::Window, window_flags);

		if (SDL_SetWindowFullscreen(_handle, flags) != 0)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return;
		}

		SDL_SetWindowSize(_handle, size.x, size.y);
		_size = size;
	}

	void SDL2Display::set_fullscreen()
	{
		Vector2i size;
		const auto flags = make_flags(size, DisplayMode::Fullscreen, DisplayWindowFlags::Zero);

		if (SDL_SetWindowFullscreen(_handle, flags) != 0)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return;
		}

		update_size();
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
				//case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				update_size();
				UC_LOG_DEBUG(_logger) << "Resized to " << _size;
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

	Uint32 SDL2Display::make_flags(Vector2i& size, DisplayMode mode, DisplayWindowFlags window_flags) const
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		switch (mode)
		{
		case DisplayMode::Window:
#if !defined(UNICORE_PLATFORM_WEB)
			if (window_flags.has(DisplayWindowFlag::Borderless))
				flags |= SDL_WINDOW_BORDERLESS;

			if (window_flags.has(DisplayWindowFlag::Resizable))
				flags |= SDL_WINDOW_RESIZABLE;
#endif
			break;

		case DisplayMode::Fullscreen:
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			size = _platform.native_size();
			break;
		}

		return flags;
	}
}
#endif