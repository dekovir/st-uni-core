#include "unicore/app/Core.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/Plugin.hpp"

namespace unicore
{
	Core::Core(const CoreSettings& settings)
		: _logger("[Core] ", settings.platform.logger)
		, platform(settings.platform)
		, logger(_logger)
		, time(platform.time)
		, input(platform.input)
		, resources(platform.resources)
	{
		platform.register_module(context);
		input.register_module(context);
		resources.register_module(context);

		_modules.add(platform);
		_modules.add(input);
		_modules.add(resources);

		_modules.register_all(context);
	}

	Core::~Core()
	{
		for (const auto& plugin : _plugins)
			plugin->unregister_module(context);

		resources.unload_all();
		resources.clear();

		resources.unregister_module(context);
		platform.unregister_module(context);
		input.unregister_module(context);

		_modules.unregister_all();
	}

	void Core::update()
	{
		platform.poll_events();

		if (time.delta() > TimeSpanConst::Zero)
			on_update();
	}

	void Core::add_plugin(Unique<Plugin>&& plugin)
	{
		_modules.add(*plugin);
		_plugins.push_back(std::move(plugin));
	}
}