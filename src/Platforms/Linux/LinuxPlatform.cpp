#include "LinuxPlatform.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "Platforms/Posix/PosixFileProvider.hpp"

namespace unicore
{
	LinuxPlatform::LinuxPlatform()
		: Platform({ _looper, _logger, _time, _input, _file_system })
		, _input_logger("[Input] ", _logger)
		, _input(_input_logger)
		, _file_system_logger("[FS] ", _logger)
		, _file_system(_file_system_logger)
	{
		file_system.add_read(std::make_shared<PosixFileProvider>(_file_system_logger));
	}

	void LinuxPlatform::update()
	{
		Platform::update();

		_time.update();
		_input.update();
	}
}
#endif