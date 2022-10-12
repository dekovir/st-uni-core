#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "unicore/io/FileSystem.hpp"
#include "WinLogger.hpp"

#if defined(UNICORE_USE_SDL2)
#include "Platforms/SDL2/SDL2Time.hpp"
#include "Platforms/SDL2/SDL2Input.hpp"
#endif

namespace unicore
{
	class WinPlatform : public Platform
	{
		UC_OBJECT(WinPlatform, Platform)
	public:
		WinPlatform();

		UC_NODISCARD bool running() const override;
		void quit() override;

		void poll_events() override;

	protected:
		WinLogger _logger;
		ProxyLogger _input_logger;

#if defined(UNICORE_USE_SDL2)
		SDL2Time _time;
		SDL2Input _input;
		bool _running = true;
#endif

		ProxyLogger _file_system_logger;
		FileSystem _file_system;
	};
}
#endif