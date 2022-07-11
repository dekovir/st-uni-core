#pragma once
#include "unicore/Render2D.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	class Logger;
	class Time;
	class Input;

	struct PlatformSettings
	{
		Logger& logger;
		Time& time;
		Input& input;
	};

	class Platform : public Module
	{
		UC_OBJECT(Platform, Module)
	public:
		explicit Platform(const PlatformSettings& settings);

		Logger& logger;
		Time& time;
		Input& input;

		ProxyLogger resources_logger;
		ResourceCache resources;

		UC_NODISCARD virtual bool running() const = 0;
		virtual void quit() = 0;

		virtual void poll_events() = 0;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

		static Unique<Platform> create();
	};
}