#include "SDL2Display.hpp"
#include "SDL2Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL_syswm.h>

namespace unicore
{
	static const char* window_data_name = "user_data";

	SDL2Display::SDL2Display(SDL2Looper& looper, const DisplaySettings& settings)
		: ParentType(settings)
		, _looper(looper)
		, _handle(nullptr)
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

		_mode = settings.mode;
		const auto flags = make_flags(_mode);

		_handle = SDL_CreateWindow(settings.title.data(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_mode.window_size.x, _mode.window_size.y, flags);

		if (!_handle)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return;
		}

		SDL_SetWindowData(_handle, window_data_name, this);
		update_mode();

		_looper.add_listener(this);
	}

	SDL2Display::~SDL2Display()
	{
		_looper.remove_listener(this);
		SDL_DestroyWindow(_handle);
	}

	void SDL2Display::set_mode(const DisplayMode& mode)
	{
		auto new_mode = mode;
		const auto flags = make_flags(new_mode);

		if (mode.fullscreen)
		{
			if (SDL_SetWindowFullscreen(_handle, flags) != 0)
			{
				UC_LOG_ERROR(_logger) << SDL_GetError();
				return;
			}

			_mode = new_mode;
		}
		else
		{
			if (SDL_SetWindowFullscreen(_handle, flags) != 0)
			{
				UC_LOG_ERROR(_logger) << SDL_GetError();
				return;
			}

			_mode = new_mode;
			SDL_SetWindowSize(_handle, _mode.window_size.x, _mode.window_size.y);
			SDL_SetWindowPosition(_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	Vector2i SDL2Display::get_maximum_size() const
	{
		Vector2i size;
		SDL_GetWindowMaximumSize(_handle, &size.x, &size.y);
		if (size.x == 0 || size.y == 0)
		{
			UC_LOG_WARNING(_logger) << "Failed to get SDL_GetWindowMaximumSize";

			SDL_DisplayMode mode;
			if (SDL_GetDesktopDisplayMode(0, &mode) == 0)
			{
				size.x = mode.w - 200;
				size.y = mode.h - 200;
				return size;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
		}

		return size;
	}

	Vector2i SDL2Display::get_minimum_size() const
	{
		Vector2i size;
		SDL_GetWindowMinimumSize(_handle, &size.x, &size.y);
		if (size.x == 0 || size.y == 0)
		{
			UC_LOG_WARNING(_logger) << "Failed to get SDL_GetWindowMinimumSize";
			size.x = 100;
			size.y = 100;
		}

		return size;
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
				update_mode();
				UC_LOG_DEBUG(_logger) << "Resized to " << _mode.window_size;
				break;
			}

			return true;
		}

		return false;
	}

	// PROTECTED /////////////////////////////////////////////////////////////////
	void SDL2Display::update_mode()
	{
		SDL_GetWindowSize(_handle, &_mode.window_size.x, &_mode.window_size.y);
		const auto flags = SDL_GetWindowFlags(_handle);

		_mode.window_flags = DisplayWindowFlags::Zero;
		if (flags & SDL_WINDOW_BORDERLESS)
			_mode.window_flags |= DisplayWindowFlag::Borderless;

		if (flags & SDL_WINDOW_RESIZABLE)
			_mode.window_flags |= DisplayWindowFlag::Resizable;
	}

	Uint32 SDL2Display::make_flags(DisplayMode& mode) const
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (mode.fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

			SDL_DisplayMode display_mode;
			if (SDL2Utils::get_desktop_native_mode(display_mode))
				mode.window_size = { display_mode.w, display_mode.h };
			else
			{
				UC_LOG_ERROR(_logger) << "Failed to get desktop display mode";
				mode.window_size = {0, 0};
			}
			mode.window_flags = DisplayWindowFlags::Zero;
		}
		else
		{
#if !defined(UNICORE_PLATFORM_WEB)
			if (mode.window_flags.has(DisplayWindowFlag::Borderless))
				flags |= SDL_WINDOW_BORDERLESS;

			if (mode.window_flags.has(DisplayWindowFlag::Resizable))
				flags |= SDL_WINDOW_RESIZABLE;
#endif
		}

		return flags;
	}
}
#endif