#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinLogger.hpp"
#include "WinStreamProvider.hpp"

#if defined(UNICORE_USE_SDL2)
#include "../SDL2/SDL2Time.hpp"
#include "../SDL2/SDL2Input.hpp"
#endif

namespace unicore
{
	class WinPlatform : public Platform
	{
	public:
		WinPlatform();

		UC_NODISCARD bool running() const override;
		void poll_events() override;

	protected:
		WinLogger _logger;
#if defined(UNICORE_USE_SDL2)
		SDL2Time _time;
		SDL2Input _input;
		bool _running = true;
#endif
		ProxyLogger _provider_logger;
		WinStreamProvider _provider;
	};
}
#endif