#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/io/FileSystem.hpp"
#include "SDL2Looper.hpp"
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

		UC_NODISCARD Vector2i native_size() const override;

		Unique<Display> create_display(const DisplaySettings& settings) override;

		void update() override;

	protected:
		SDL2Looper _looper;

#if defined(UNICORE_PLATFORM_WINDOWS)
		WinLogger _logger;
#else
		PrintLogger _logger;
#endif

		ProxyLogger _input_logger;

		SDL2Time _time;
		SDL2Input _input;

		ProxyLogger _file_system_logger;
		FileSystem _file_system;
	};
}
#endif