#pragma once
#include "unicore/ResourceCache.hpp"
#include "unicore/Display.hpp"

namespace unicore
{
	class Time;
	class Input;

	class Platform : public Module
	{
		UC_OBJECT(Platform, Module)
	public:
		struct Settings
		{
			Logger& logger;
			Time& time;
			Input& input;
		};

		explicit Platform(const Settings& settings);

		Logger& logger;
		Time& time;
		Input& input;

		ProxyLogger resources_logger;
		ResourceCache resources;

		UC_NODISCARD virtual const Vector2i& native_size() const = 0;

		virtual Unique<Display> create_display(const DisplaySettings& settings) = 0;

		UC_NODISCARD virtual bool running() const = 0;
		virtual void quit() = 0;

		virtual void poll_events() = 0;

		void register_module(const ModuleContext& context) override;

		static Unique<Platform> create();
	};
}