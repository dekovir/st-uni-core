#include "WinPlatform.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)

namespace unicore
{
	WinPlatform::WinPlatform()
		: Platform({ _logger, _time, _input })
		, _provider_logger("[FS] ", _logger)
		, _provider(_provider_logger)
	{
		resources.add_provider(_provider);
	}

	bool WinPlatform::running() const
	{
#if defined(UNICORE_USE_SDL2)
		return _running;
#else
		UC_STATIC_ASSERT_ALWAYS("Unimplemented platform");
#endif
	}

	void WinPlatform::poll_events()
	{
#if defined(UNICORE_USE_SDL2)
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				_running = false;
				break;

			case SDL_WINDOWEVENT:
				//_log.debug("Evt: Wnd: " + Strings::From(event.window.event));
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					logger.debug("Resized");
					//_render.update_size();
					break;
				}
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					_running = false;
				break;

			case SDL_MOUSEMOTION:
				_input.apply_event(event.motion);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				_input.apply_event(event.button);
				break;
			}
		}

		_time.update();
#else
		UC_STATIC_ASSERT_ALWAYS("Unimplemented platform");
#endif
	}
}
#endif