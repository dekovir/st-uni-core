#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Time.hpp"
#include "SDL2Input.hpp"
#include "SDL2Display.hpp"

#if defined(UNICORE_PLATFORM_WINDOWS)
#	include "Platforms/Windows/WinLogger.hpp"
#endif

namespace unicore
{
	class SDL2Platform : public Platform
	{
		UC_OBJECT(SDL2Platform, Platform)
	public:
		SDL2Platform();

		UC_NODISCARD const Vector2i& native_size() const override { return _native_size; }

		Unique<Display> create_display(const DisplaySettings& settings) override;

		UC_NODISCARD bool running() const override;
		void quit() override;

		void poll_events() override;

		void add_listener(SDL2EventListener* listener);
		void remove_listener(SDL2EventListener* listener);

	protected:
#if defined(UNICORE_PLATFORM_WINDOWS)
		WinLogger _logger;
#else
		PrintLogger _logger;
#endif

		ProxyLogger _input_logger;

		SDL2Time _time;
		SDL2Input _input;
		bool _running = true;
		ProxyLogger _provider_logger;

		Vector2i _native_size = VectorConst2i::Zero;

		Set<SDL2EventListener*> _listeners;

		void update_native_size();
	};
}
#endif