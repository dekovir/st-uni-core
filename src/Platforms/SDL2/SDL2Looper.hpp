#pragma once
#include "unicore/Looper.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2Looper : public Looper
	{
	public:
		UC_NODISCARD bool running() const override { return _running; }

		void quit() override;
		void poll_events() override;

		void add_listener(SDL2EventListener* listener);
		void remove_listener(SDL2EventListener* listener);

	protected:
		bool _running = true;
		Set<SDL2EventListener*> _listeners;
	};
}
#endif