#include "unicore/app/SDLCore.hpp"

namespace unicore
{
	// TODO: Refactor this
	namespace details
	{
		Unique<RendererSDL> renderer;

		RendererSDL& create_renderer(Logger& logger, Display& display)
		{
			if (renderer) return *renderer;

			renderer = RendererSDL::create(logger, display);
			if (!renderer)
				UC_LOG_ERROR(logger) << "Failed to create RendererSDL";

			return *renderer;
		}
	}

	SDLCore::SDLCore(const DisplayCoreSettings& settings)
		: RendererCoreT(settings, &details::create_renderer)
	{
	}

	SDLCore::~SDLCore()
	{
		// TODO: Implement delete logic
		//details::renderer = nullptr;
	}
}