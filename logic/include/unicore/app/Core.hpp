#pragma once
#include "unicore/Platform.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	class Plugin;

	class Core
	{
	public:
		struct Settings
		{
			Platform& platform;
		};

		explicit Core(const Settings& settings);
		virtual ~Core();

		Platform& platform;
		Logger& logger;
		Time& time;
		Input& input;
		ResourceCache& resources;

		Context context;

		virtual void update();

	protected:
		virtual void on_update() = 0;

	private:
		List<Unique<Plugin>> _plugins;
	};
}