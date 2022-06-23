#pragma once
#include "unicore/Logger.hpp"
#include "unicore/FileSystem.hpp"
#include "unicore/Input.hpp"
#include "unicore/Render2D.hpp"
#include "unicore/Time.hpp"

namespace unicore
{
	struct PlatformSettings
	{
		Time& time;
		Input& input;
		Render2D& render;
	};

	class Platform : public Module
	{
	public:
		explicit Platform(const PlatformSettings& settings);

		Logger& logger;
		Time& time;
		Input& input;
		Render2D& render;
		FileSystem file_system;

		UC_NODISCARD virtual bool running() const = 0;

		virtual void poll_events() = 0;

		static Shared<Platform> create();
	};
}