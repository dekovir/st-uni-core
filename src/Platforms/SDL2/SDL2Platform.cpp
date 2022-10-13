#include "SDL2Platform.hpp"
#if defined(UNICORE_USE_SDL2)

#if defined(UNICORE_PLATFORM_WINDOWS)
#	include "Platforms/Windows/WinFileProvider.hpp"
#elif defined(UNICORE_PLATFORM_POSIX)
#	include "Platforms/Posix/PosixFileProvider.hpp"
#endif

namespace unicore
{
	SDL2Platform::SDL2Platform()
		: Platform({ _looper, _logger, _time, _input, _file_system })
		, _input_logger("[Input] ", _logger)
		, _looper(_logger)
		, _input(_input_logger)
		, _file_system_logger("[FS] ", _logger)
		, _file_system(_file_system_logger)
	{
#if defined(UNICORE_PLATFORM_WINDOWS)
		file_system.add_read(std::make_shared<WinFileProvider>(_file_system_logger));
#elif defined(UNICORE_PLATFORM_POSIX)
		file_system.add_read(std::make_shared<PosixFileProvider>(_file_system_logger));
#endif
	}

	Vector2i SDL2Platform::native_size() const
	{
		SDL_DisplayMode mode;
		if (SDL_GetDesktopDisplayMode(0, &mode) == 0)
		{
			return{ mode.w, mode.h };
		}
		return { 0, 0 };
	}

	Unique<Display> SDL2Platform::create_display(const DisplaySettings& settings)
	{
		return std::make_unique<SDL2Display>(_looper, settings);
	}

	void SDL2Platform::update()
	{
		Platform::update();

		_time.update();
		_input.update();
	}
}
#endif