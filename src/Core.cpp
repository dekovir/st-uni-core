#include "unicore/Core.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"

namespace unicore
{
	Core::Core(const CoreSettings& settings)
		: platform(settings.platform)
		, logger(settings.platform.logger)
		, time(settings.platform.time)
		, input(settings.platform.input)
		, resources(settings.platform.resources)
		, render(settings.render)
	{
		platform.register_module(context);
		input.register_module(context);
		render.register_module(context);
		resources.register_module(context);
	}

	Core::~Core()
	{
		resources.unload_all();
		resources.clear();

		resources.unregister_module(context);
		platform.unregister_module(context);
		render.unregister_module(context);
		input.unregister_module(context);
	}

	void Core::update()
	{
		platform.poll_events();
		on_update();

		_fps_time += time.delta();
		if (_fps_time >= TimeSpanConst::OneSecond)
		{
			_fps_current = _fps_counter;

			_fps_counter = 0;
			_fps_time -= TimeSpanConst::OneSecond;
		}
	}

	void Core::draw()
	{
		_fps_counter++;
		if (render.begin_scene())
		{
			on_draw();
			render.end_scene();
		}
	}

	void Core::frame()
	{
		update();
		draw();
	}
}