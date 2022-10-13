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
		, _input(_input_logger)
		, _file_system_logger("[FS] ", _logger)
		, _file_system(_file_system_logger)
	{
		SDL_SetMainReady();
		SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

		SDL_version version;
		SDL_GetVersion(&version);
		UC_LOG_INFO(_logger) << "SDL version "
			<< version.major << "." << version.minor << "." << version.minor;

		const auto display_count = SDL_GetNumVideoDisplays();
		if (display_count >= 1)
		{
			for (auto display_index = 0; display_index < display_count; display_index++)
			{
				float ddpi, hdpi, vdpi;
				if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) != 0)
				{
					UC_LOG_WARNING(_logger) << SDL_GetError();
					ddpi = 0; hdpi = 0; vdpi = 0;
				}

				SDL_Rect bounds;
				SDL_GetDisplayBounds(display_index, &bounds);
				SDL_DisplayMode mode;

				SDL_GetCurrentDisplayMode(display_index, &mode);

				auto display_orientation = SDL_GetDisplayOrientation(display_index);

				SDL_Rect usable_bounds;
				SDL_GetDisplayUsableBounds(display_index, &usable_bounds);

				UC_LOG_INFO(_logger)
					<< "Display " << display_index << ": " << SDL_GetDisplayName(display_index)
					<< " " << mode.w << "x" << mode.h << " " << mode.refresh_rate
					<< (Math::equals(ddpi, hdpi) && Math::equals(hdpi, vdpi)
						? StringBuilder::format(" ({})", ddpi)
						: StringBuilder::format(" ({},{},{})", ddpi, hdpi, vdpi))
					<< " " << bounds << " " << usable_bounds << " " << display_orientation;
			}
		}

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