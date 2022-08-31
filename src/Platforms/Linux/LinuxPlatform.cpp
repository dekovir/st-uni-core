#include "LinuxPlatform.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "LinuxStreamProvider.hpp"

namespace unicore
{
	LinuxPlatform::LinuxPlatform()
	: Platform({ _logger, _time, _input })
		, _input_logger("[Input] ", _logger)
		, _input(_input_logger)
		, _provider_logger("[FS] ", _logger)
	{
		resources.mount(std::make_shared<LinuxStreamProvider>(_provider_logger));
	}

	bool LinuxPlatform::running() const
	{
#if defined(UNICORE_USE_SDL2)
		return _running;
#else
		UC_STATIC_ASSERT_ALWAYS("Unimplemented");
#endif
	}

	void LinuxPlatform::quit()
	{
		_running = false;
	}

	void LinuxPlatform::poll_events()
	{
#if defined(UNICORE_USE_SDL2)
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
#else
		UC_STATIC_ASSERT_ALWAYS("Unimplemented");
#endif
	}
}
#endif