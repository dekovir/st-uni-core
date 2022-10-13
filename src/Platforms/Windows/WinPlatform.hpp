#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "unicore/io/FileSystem.hpp"
#include "WinLogger.hpp"

#if defined(UNICORE_USE_SDL2)
#include "Platforms/SDL2/SDL2Looper.hpp"
#include "Platforms/SDL2/SDL2Time.hpp"
#include "Platforms/SDL2/SDL2Input.hpp"
#include "Platforms/SDL2/SDL2Display.hpp"
#endif

namespace unicore
{
	class WinPlatform : public Platform
	{
		UC_OBJECT(WinPlatform, Platform)
	public:
		WinPlatform();

		void update() override;

		UC_NODISCARD Vector2i native_size() const override;
		Unique<Display> create_display(const DisplaySettings& settings) override;

	protected:
		WinLogger _logger;
		ProxyLogger _input_logger;

#if defined(UNICORE_USE_SDL2)
		SDL2Looper _looper;
		SDL2Time _time;
		SDL2Input _input;
#endif

		ProxyLogger _file_system_logger;
		FileSystem _file_system;
	};
}
#endif