#include "unicore/platform/Platform.hpp"
#include "unicore/io/Logger.hpp"

#if defined(UNICORE_PLATFORM_WINDOWS)
#	include "Platforms/Windows/WinPlatform.hpp"
#elif defined(UNICORE_PLATFORM_LINUX)
#	include "Platforms/Linux/LinuxPlatform.hpp"
#endif

namespace unicore
{
	Platform::Platform(const Settings& settings)
		: looper(settings.looper)
		, logger(settings.logger)
		, time(settings.time)
		, input(settings.input)
		, file_system(settings.file_system)
		, resources_logger("[Cache] ", logger)
		, resources(resources_logger)
	{
	}

	void Platform::update()
	{
		looper.poll_events();
	}

	Unique<Platform> Platform::create()
	{
#if defined(UNICORE_PLATFORM_WINDOWS)
		return make_unique<WinPlatform>();
#elif defined(UNICORE_PLATFORM_LINUX)
		return make_unique<LinuxPlatform>();
#else
		static_assert(true, "Unknown platform");
#endif
	}
}