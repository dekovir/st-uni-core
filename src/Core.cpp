#include "unicore/Core.hpp"

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
		time.register_module(context);
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
		time.unregister_module(context);
	}

	void Core::update()
	{
		platform.poll_events();
		on_update();
	}

	void Core::draw()
	{
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