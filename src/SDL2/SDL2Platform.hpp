#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Time.hpp"
#include "SDL2Input.hpp"

#if defined(UNICORE_PLATFORM_WINDOWS)
#	include "../Windows/WinLogger.hpp"
#	include "../Windows/WinStreamProvider.hpp"
#elif defined(UNICORE_PLATFORM_LINUX)
#	include "../Linux/LinuxStreamProvider.hpp"
#else
UC_STATIC_ASSERT_ALWAYS("Unsupported platform")
#endif

namespace unicore
{
	class SDL2Platform : public Platform
	{
		UC_OBJECT(SDL2Platform, Platform)
	public:
		SDL2Platform();

		UC_NODISCARD bool running() const override;
		void quit() override;

		void poll_events() override;

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

#if defined(UNICORE_PLATFORM_WINDOWS)
		WinStreamProvider _provider;
#elif defined(UNICORE_PLATFORM_LINUX)
		LinuxStreamProvider _provider;
#endif
	};
}
#endif