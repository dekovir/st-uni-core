#pragma once
#include "unicore/platform/Looper.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class Logger;

	class SDL2Looper : public Looper
	{
	public:
		explicit SDL2Looper(Logger& logger);

		UC_NODISCARD bool running() const override { return _running; }

		void quit() override;

		void poll_events();

		void add_listener(SDL2EventListener* listener);
		void remove_listener(SDL2EventListener* listener);

	protected:
		Logger& _logger;
		bool _running = true;
		Set<SDL2EventListener*> _listeners;
	};
}
#endif