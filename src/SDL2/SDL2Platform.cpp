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
		SDL_SetMainReady();
		SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

		SDL_version version;
		SDL_GetVersion(&version);
		UC_LOG_INFO(_logger) << "SDL version "
			<< version.major << "." << version.minor << "." << version.minor;

		update_native_size();

		resources.add_provider(_provider);
	}

	Unique<Display> SDL2Platform::create_display(const DisplaySettings& settings)
	{
		return std::make_unique<SDL2Display>(*this, settings);
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
			}

			for (const auto listener : _listeners)
			{
				if (listener->on_event(evt))
					break;
			}
		}

		_time.update();
		_input.update();
	}

	void SDL2Platform::add_listener(SDL2EventListener* listener)
	{
		_listeners.insert(listener);
	}

	void SDL2Platform::remove_listener(SDL2EventListener* listener)
	{
		_listeners.erase(listener);
	}

	void SDL2Platform::update_native_size()
	{
		SDL_DisplayMode mode;
		if (SDL_GetDesktopDisplayMode(0, &mode) == 0)
		{
			_native_size.x = mode.w;
			_native_size.y = mode.h;
		}
		else
		{
			_native_size.set(0, 0);
			UC_LOG_ERROR(_logger) << SDL_GetError();
		}
	}
}
#endif