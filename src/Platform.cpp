#include "unicore/Platform.hpp"
#include "unicore/Data.hpp"
#include "unicore/Font.hpp"
#include "SDL2/SDL2Platform.hpp"

namespace unicore
{
	Platform::Platform(const PlatformSettings& settings)
		: logger(Logger::get_native())
		, time(settings.time)
		, input(settings.input)
		, render(settings.render)
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
	}

	void Platform::unregister_module(Context& context)
	{
		Module::unregister_module(context);
	}

	Shared<Platform> Platform::create()
	{
#if defined(UNICORE_USE_SDL2)
		unicore::SDLPlatformSettings settings;
#if defined(UNICORE_PLATFORM_EMSCRIPTEN)
#	if 0
		emscripten_get_screen_size(
			&settings.render.size.x,
			&settings.render.size.y);
#	else
		settings.render.size.x = 800;
		settings.render.size.y = 600;
#	endif
#else
		settings.render.size = unicore::Vector2i::Zero;
		//settings.render.size = unicore::Vector2i(800, 600);
		settings.render.resizeable = false;
		settings.render.borderless = false;
		settings.render.fullscreen = false;
#endif

		return make_shared<SDL2Platform>(settings);
#else
		static_assert(true, "Unknown platform");
#endif
	}
}