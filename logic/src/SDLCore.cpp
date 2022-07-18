#include "unicore/app/SDLCore.hpp"

namespace unicore
{
	// TODO: Refactor this
	namespace details
	{
		static Shared<Logger> logger;
		static Shared<SDLRenderer> renderer;

		static SDLRenderer& get_sdl_renderer(const Core::Settings& settings)
		{
			if (!renderer)
			{
				logger = std::make_shared<ProxyLogger>("[Renderer] ", settings.platform.logger);
				renderer = SDLRenderer::create(*logger);
			}

			return *renderer;
		}

		static void remove_sdl_renderer()
		{
			renderer = nullptr;
			logger = nullptr;
		};
	}

	SDLCore::SDLCore(const Settings& settings)
		: RendererCoreT<SDLRenderer>(settings, details::get_sdl_renderer(settings))
	{
	}

	SDLCore::~SDLCore()
	{
		// TODO: Implement delete logic
		//details::remove_sdl_renderer();
	}
}