#include "unicore/Platform.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Input.hpp"
#include "unicore/Time.hpp"
#include "unicore/BinaryData.hpp"
#include "unicore/StringData.hpp"
#if defined(UNICORE_USE_SDL2)
#	include "SDL2/SDL2Platform.hpp"
#elif defined(UNICORE_PLATFORM_WINDOWS)
#	include "Windows/WinPlatform.hpp"
#elif defined(UNICORE_PLATFORM_LINUX)
#	include "Linux/LinuxPlatform.hpp"
#endif

namespace unicore
{
	Platform::Platform(const Settings& settings)
		: logger(settings.logger)
		, time(settings.time)
		, input(settings.input)
		, resources_logger("[Cache] ", logger)
		, resources(resources_logger)
	{
	}

	void Platform::register_module(Context& context)
	{
		Module::register_module(context);

		static BinaryDataLoader binary_data_loader;
		static StringDataLoader string_data_loader;

		context.add_loader(binary_data_loader);
		context.add_loader(string_data_loader);
	}

	void Platform::unregister_module(Context& context)
	{
		Module::unregister_module(context);
	}

	Unique<Platform> Platform::create()
	{
#if defined(UNICORE_USE_SDL2)
		return make_unique<SDL2Platform>();
#elif defined(UNICORE_PLATFORM_WINDOWS)
		return make_unique<WinPlatform>();
		//#elif defined(UNICORE_PLATFORM_EMSCRIPTEN)
#elif defined(UNICORE_PLATFORM_LINUX)
		return make_unique<LinuxPlatform>();
#else
		static_assert(true, "Unknown platform");
#endif
	}
}