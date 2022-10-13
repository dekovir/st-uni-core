#include "SDL2Looper.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	void SDL2Looper::quit()
	{
		_running = false;
	}

	void SDL2Looper::poll_events()
	{
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				_running = false;
				break;
			}

			for (const auto listener : _listeners)
			{
				if (listener->on_event(evt))
					break;
			}
		}
	}

	void SDL2Looper::add_listener(SDL2EventListener* listener)
	{
		_listeners.insert(listener);
	}

	void SDL2Looper::remove_listener(SDL2EventListener* listener)
	{
		_listeners.erase(listener);
	}
}
#endif