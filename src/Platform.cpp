#include "unicore/Platform.hpp"
#include "unicore/Data.hpp"
#include "unicore/XMLData.hpp"
#include "unicore/Font.hpp"
#include "Windows/WinPlatform.hpp"
#include "Linux/LinuxPlatform.hpp"

namespace unicore
{
	Platform::Platform(const PlatformSettings& settings)
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

		static BinaryDataLoader data_loader;
		static TextDataLoader text_loader;
		static BitmapFontLoader font_loader;

		context.add_loader(data_loader);
		context.add_loader(text_loader);
		context.add_loader(font_loader);

#if defined(UNICORE_USE_XML)
		static XMLDataLoader xml_loader;
		context.add_loader(xml_loader);
#endif
	}

	void Platform::unregister_module(Context& context)
	{
		Module::unregister_module(context);
	}

	Unique<Platform> Platform::create()
	{
#if defined(UNICORE_PLATFORM_WINDOWS)
		return make_unique<WinPlatform>();
//#elif defined(UNICORE_PLATFORM_EMSCRIPTEN)
#elif defined(UNICORE_PLATFORM_LINUX)
		return make_unique<LinuxPlatform>();
#else
		static_assert(true, "Unknown platform");
#endif
	}
}