#include "unicore/app/Core.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"

#if defined(UNICORE_USE_XML)
#	include "unicore/xml/XMLPlugin.hpp"
#endif

#if defined(UNICORE_USE_FNT)
#	include "unicore/fnt/FNTPlugin.hpp"
#endif

namespace unicore
{
	Core::Core(const CoreSettings& settings)
		: platform(settings.platform)
		, logger(settings.platform.logger)
		, time(settings.platform.time)
		, input(settings.platform.input)
		, resources(settings.platform.resources)
		, renderer(settings.render)
	{
		platform.register_module(context);
		input.register_module(context);
		renderer.register_module(context);
		resources.register_module(context);

#if defined(UNICORE_USE_XML)
		_plugins.push_back(std::make_unique<XMLPlugin>());
#endif

#if defined(UNICORE_USE_FNT)
		_plugins.push_back(std::make_unique<FNTPlugin>());
#endif

		for (const auto& plugin : _plugins)
			plugin->register_module(context);
	}

	Core::~Core()
	{
		for (const auto& plugin : _plugins)
			plugin->unregister_module(context);

		resources.unload_all();
		resources.clear();

		resources.unregister_module(context);
		platform.unregister_module(context);
		renderer.unregister_module(context);
		input.unregister_module(context);
	}

	void Core::update()
	{
		platform.poll_events();

		if (time.delta() > TimeSpanConst::Zero)
		{
			on_update();

			_fps_time += time.delta();
			if (_fps_time >= TimeSpanConst::OneSecond)
			{
				_fps_current = _fps_counter;

				_fps_counter = 0;
				_fps_time -= TimeSpanConst::OneSecond;
			}
		}
	}

	void Core::draw()
	{
		_fps_counter++;
		if (renderer.begin_scene())
		{
			on_draw();
			renderer.end_scene();
		}
	}

	void Core::frame()
	{
		update();
		draw();
	}
}