#include "SDL2Time.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	SDL2Time::SDL2Time()
	{
		SDL_InitSubSystem(SDL_INIT_TIMER);
		_prevTicks = SDL_GetTicks();
	}

	void SDL2Time::update()
	{
		const auto ticks = SDL_GetTicks();
		const auto delta = ticks - _prevTicks;

		_delta = TimeSpan::from_milliseconds(delta);
		_elapsed += _delta;
		_prevTicks = ticks;
	}
}
#endif