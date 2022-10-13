#pragma once
#include "unicore/Module.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Display.hpp"
#include "unicore/Looper.hpp"

namespace unicore
{
	class Time;
	class Input;
	class FileSystem;

	class Platform : public Module
	{
		UC_OBJECT(Platform, Module)
	public:
		struct Settings
		{
			Looper& looper;
			Logger& logger;
			Time& time;
			Input& input;
			FileSystem& file_system;
		};

		explicit Platform(const Settings& settings);

		Looper& looper;
		Logger& logger;
		Time& time;
		Input& input;
		FileSystem& file_system;

		ProxyLogger resources_logger;
		ResourceCache resources;

		virtual void update();

		UC_NODISCARD virtual const Vector2i& native_size() const = 0;

		virtual Unique<Display> create_display(const DisplaySettings& settings) = 0;

		static Unique<Platform> create();
	};
}