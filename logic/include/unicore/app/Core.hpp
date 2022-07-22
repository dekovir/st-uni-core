#pragma once
#include "unicore/Platform.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	class Plugin;

	struct CoreSettings
	{
		Platform& platform;
	};

	class Core
	{
		ProxyLogger _logger;
	public:
		explicit Core(const CoreSettings& settings);
		virtual ~Core();

		Platform& platform;
		Logger& logger;
		Time& time;
		Input& input;
		ResourceCache& resources;

		Context context;

		virtual void update();

		void add_plugin(Unique<Plugin>&& plugin);

		template<typename TPlugin,
			std::enable_if_t<std::is_base_of_v<Plugin, TPlugin>>* = nullptr>
		void add_plugin()
		{
			add_plugin(std::make_unique<TPlugin>());
		}

	protected:
		virtual void on_update() = 0;

	private:
		bool _plugins_registered = false;
		List<Unique<Plugin>> _plugins;
	};
}