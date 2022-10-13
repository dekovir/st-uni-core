#include "WinPlatform.hpp"

#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinFileProvider.hpp"

namespace unicore
{
	WinPlatform::WinPlatform()
		: Platform({ _looper, _logger, _time, _input, _file_system })
		, _input_logger("[Input] ", _logger)
		, _looper(_logger)
		, _input(_input_logger)
		, _file_system_logger("[FS] ", _logger)
		, _file_system(_file_system_logger)
	{
		file_system.add_read(std::make_shared<WinFileProvider>(_file_system_logger));
	}

	void WinPlatform::update()
	{
		Platform::update();

		_time.update();
		_input.update();
	}

	Vector2i WinPlatform::native_size() const
	{
#if defined(UNICORE_USE_SDL2)
		SDL_DisplayMode mode;
		if (SDL2Utils::get_desktop_native_mode(mode))
			return { mode.w, mode.h };
#endif

		UC_ASSERT_ALWAYS_MSG("Unimplemented");
		return { 0, 0 };
	}

	Unique<Display> WinPlatform::create_display(const DisplaySettings& settings)
	{
#if defined(UNICORE_USE_SDL2)
		return std::make_unique<SDL2Display>(_looper, settings);
#endif
	}
}
#endif