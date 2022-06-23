#include "SDL2Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/LogHelper.hpp"
#include "SDL2Utils.hpp"
#include "SDL2StreamProvider.hpp"
#include "SDL2Loaders.hpp"
#if defined(EMSCRIPTEN)
#	include <emscripten/emscripten.h>
#endif

namespace unicore
{
	SDL2Timer::SDL2Timer()
	{
		_prevTicks = SDL_GetTicks();
	}

	void SDL2Timer::Update()
	{
		const auto ticks = SDL_GetTicks();
		const auto delta = ticks - _prevTicks;

		_delta = TimeSpan::from_milliseconds(delta);
		_elapsed += _delta;
		_prevTicks = ticks;
	}

	// ===========================================================================
	SDL2Platform::SDL2Platform(const SDLPlatformSettings& settings)
		: Platform({ _time, _input, _render })
		, _render_logger("[Render] ", logger)
		, _render(_render_logger, settings.render)
	{
		file_system.add_provider(std::make_shared<SDL2StreamProvider>());

		SDL_version ver;
		SDL_GetVersion(&ver);
		UC_LOG_INFO(logger) << L"SDL version " << ver;
	}

	SDL2Platform::~SDL2Platform()
	{
		SDL_Quit();
	}

	void SDL2Platform::poll_events()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				_running = false;
				break;

			case SDL_WINDOWEVENT:
				//_log.debug("Evt: Wnd: " + Strings::From(event.window.event));
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					logger.debug("Resized");
					_render.update_size();
					break;
				}
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					_running = false;
				break;

			case SDL_MOUSEMOTION:
				_input.ApplyEvent(event.motion);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				_input.ApplyEvent(event.button);
				break;
			}
		}

		_time.Update();
	}

	void SDL2Platform::register_module(Context& context)
	{
		Platform::register_module(context);

		static SDL2SurfaceLoader loader;
		context.add_loader(loader);
	}
}
#endif