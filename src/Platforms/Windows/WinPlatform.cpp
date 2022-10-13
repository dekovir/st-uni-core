#include "WinPlatform.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinFileProvider.hpp"

namespace unicore
{
	WinPlatform::WinPlatform()
		: Platform({ _looper, _logger, _time, _input, _file_system })
		, _input_logger("[Input] ", _logger)
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
}
#endif