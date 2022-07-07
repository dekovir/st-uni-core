#include "SDL2Platform.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	SDL2Platform::SDL2Platform()
		: Platform({ _logger, _time, _input })
		, _input_logger("[Input] ", _logger)
		, _input(_input_logger)
		, _provider_logger("[FS] ", _logger)
		, _provider(_provider_logger)
	{
		resources.add_provider(_provider);
	}

	bool SDL2Platform::running() const
	{
		return _running;
	}

	void SDL2Platform::quit()
	{
		_running = false;
	}

	void SDL2Platform::poll_events()
	{
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				_running = false;
				break;

			case SDL_WINDOWEVENT:
				//_log.debug("Evt: Wnd: " + Strings::From(event.window.event));
				switch (evt.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					UC_LOG_DEBUG(logger) << "Resized";
					//_render.update_size();
					break;
				}
				break;
			}
		}

		_time.update();
		_input.update();
	}
}
#endif