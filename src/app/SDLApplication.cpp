#include "unicore/app/SDLApplication.hpp"
#include "Platforms/SDL2/SDL2RendererSDL.hpp"

namespace unicore
{
	// TODO: Refactor this
	namespace details
	{
		Unique<sdl2::Pipeline> renderer;

		sdl2::Pipeline& create_renderer(Logger& logger, Display& display)
		{
			if (renderer) return *renderer;

#if defined(UNICORE_USE_SDL2)
			renderer = SDL2RendererSDL::create(logger, display);
#else
			UC_STATIC_ASSERT_ALWAYS("Unimplemented platform");
#endif

			if (!renderer)
				UC_LOG_ERROR(logger) << "Failed to create RendererSDL";

			return *renderer;
		}
	}

	SDLApplication::SDLApplication(const DisplayCoreSettings& settings)
		: RendererCoreT(settings, &details::create_renderer)
	{
	}

	SDLApplication::~SDLApplication()
	{
		// TODO: Implement delete logic
		//details::renderer = nullptr;
	}
}