#pragma once
#include "unicore/Logger.hpp"
#include "unicore/FileSystem.hpp"
#include "unicore/Input.hpp"
#include "unicore/Render.hpp"
#include "unicore/Time.hpp"

namespace unicore
{
	struct PlatformSettings
	{
		Time& time;
		Input& input;
		Render& render;
	};

	class Platform
	{
	protected:
		Shared<Logger> _system_logger;

	public:
		explicit Platform(const PlatformSettings& settings);
		virtual ~Platform() = default;

		Logger& logger;
		Time& time;
		Input& input;
		Render& render;
		FileSystem file_system;

		[[nodiscard]] virtual bool running() const = 0;

		virtual void poll_events() = 0;

		static Shared<Platform> create();
	};
}