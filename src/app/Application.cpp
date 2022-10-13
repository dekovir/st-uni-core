#include "unicore/app/Application.hpp"
#include "unicore/system/TimeSpan.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/platform/Plugin.hpp"
#include "unicore/io/FileSystem.hpp"
#include "unicore/io/FileLoader.hpp"

namespace unicore
{
	Application::Application(const CoreSettings& settings)
		: _logger("[App] ", settings.platform.logger)
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
	}

	Application::~Application()
	{
		_modules.unregister_all();
	}

	void Application::init()
	{
		_modules.register_all({ &logger, _modules });

		for (const auto& plugin : _plugins)
			plugin->register_plugin({ &logger, _modules });

		resources.add_loader(std::make_shared<ReadFileLoader>(file_system));

		on_init();
	}

	void Application::update()
	{
		platform.update();

		if (time.delta() > TimeSpanConst::Zero)
			on_update();
	}

	void Application::add_plugin(Unique<Plugin>&& plugin)
	{
		_plugins.push_back(std::move(plugin));
		UC_LOG_DEBUG(_logger) << "Add " << _plugins.back()->type();
	}
}