#include "unicore/Platform.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Input.hpp"
#include "unicore/Time.hpp"
#include "unicore/BinaryData.hpp"
#include "unicore/StringData.hpp"
#if defined(UNICORE_USE_SDL2)
#	include "Platforms/SDL2/SDL2Platform.hpp"
#elif defined(UNICORE_PLATFORM_WINDOWS)
#	include "Platforms/Windows/WinPlatform.hpp"
#elif defined(UNICORE_PLATFORM_LINUX)
#	include "Platforms/Linux/LinuxPlatform.hpp"
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

	void Platform::register_module(const ModuleContext& context)
	{
		Module::register_module(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<BinaryDataLoader>());
			cache->add_loader(std::make_shared<StringDataLoader>());
		}
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