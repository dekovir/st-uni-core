#include "unicore/Platform.hpp"
#include "unicore/Data.hpp"
#include "unicore/Font.hpp"
#include "Windows/WinPlatform.hpp"

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
		static XMLDataLoader xml_loader;

		context.add_loader(data_loader);
		context.add_loader(text_loader);
		context.add_loader(font_loader);
		context.add_loader(xml_loader);
	}

	void Platform::unregister_module(Context& context)
	{
		Module::unregister_module(context);
	}

	Shared<Platform> Platform::create()
	{
#if defined(UNICORE_PLATFORM_WINDOWS)
		return std::make_shared<WinPlatform>();
//#elif defined(UNICORE_PLATFORM_EMSCRIPTEN)
#else
		static_assert(true, "Unknown platform");
#endif
	}
}