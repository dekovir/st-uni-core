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
		_modules.add(platform);
		_modules.add(input);
		_modules.add(resources);
	}

	Core::~Core()
	{
		_modules.unregister_all();
	}

	void Core::init()
	{
		_modules.register_all({ _modules });

		on_init();
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