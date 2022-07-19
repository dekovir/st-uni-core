#include "unicore/app/SDLCore.hpp"

namespace unicore
{
	// TODO: Refactor this
	namespace details
	{
		Unique<SDLRenderer> renderer;

		SDLRenderer& create_renderer(Logger& logger, Display& display)
		{
			if (!renderer)
				renderer = SDLRenderer::create(logger, display);

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