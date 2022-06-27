#include "unicore/Core.hpp"

namespace unicore
{
	Core::Core(Platform& _platform)
		: logger(_platform.logger)
		, time(_platform.time)
		, input(_platform.input)
		, render(_platform.render)
		, platform(_platform)
		, resources(_platform.resources)
	{
		time.register_module(context);
		input.register_module(context);
		render.register_module(context);
		platform.register_module(context);
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
		on_update();
	}

	void Core::draw()
	{
		on_draw();
	}
}