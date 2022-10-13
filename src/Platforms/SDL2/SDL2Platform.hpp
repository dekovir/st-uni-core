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

		UC_NODISCARD const Vector2i& native_size() const override { return _native_size; }

		Unique<Display> create_display(const DisplaySettings& settings) override;

		SDL2Looper sdl_looper;

		void update() override;

	protected:
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

		Vector2i _native_size = VectorConst2i::Zero;

		void update_native_size();
	};
}
#endif