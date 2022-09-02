#include "LinuxPlatform.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "Platforms/Posix/PosixFileProvider.hpp"

namespace unicore
{
	LinuxPlatform::LinuxPlatform()
		: Platform({ _logger, _time, _input, _file_system })
		, _input_logger("[Input] ", _logger)
		, _input(_input_logger)
		, _file_system_logger("[FS] ", _logger)
		, _file_system(_file_system_logger)
	{
		file_system.add_read(std::make_shared<PosixFileProvider>(_file_system_logger));
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