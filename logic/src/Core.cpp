#include "unicore/app/Core.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/Plugin.hpp"
#include "unicore/plugins/PlatformPlugin.hpp"
#include "unicore/plugins/SurfacePlugin.hpp"

namespace unicore
{
	Core::Core(const CoreSettings& settings)
		: _logger("[Core] ", settings.platform.logger)
		, platform(settings.platform)
		, logger(_logger)
		, time(platform.time)
		, input(platform.input)
		, file_system(platform.file_system)
		, resources(platform.resources)
	{
		_modules.add(platform);
		_modules.add(input);
		_modules.add(file_system);
		_modules.add(resources);

		create_plugin<PlatformPlugin>();
		create_plugin<SurfacePlugin>();
	}

	Core::~Core()
	{
		_modules.unregister_all();
	}

	void Core::init()
	{
		_modules.register_all({ &logger, _modules });

		for (const auto& plugin : _plugins)
			plugin->register_plugin({ &logger, _modules });

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
		_plugins.push_back(std::move(plugin));
	}
}