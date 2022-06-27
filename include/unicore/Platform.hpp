#pragma once
#include "unicore/Logger.hpp"
#include "unicore/Input.hpp"
#include "unicore/Render2D.hpp"
#include "unicore/Time.hpp"
#include "unicore/ResourceCache.hpp"

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

		ProxyLogger resources_logger;
		ResourceCache resources;

		UC_NODISCARD virtual bool running() const = 0;

		virtual void poll_events() = 0;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

		static Shared<Platform> create();
	};
}