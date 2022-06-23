#include "unicore/Core.hpp"

namespace unicore
{
	Core::Core(Platform& _platform)
		: logger(_platform.logger)
		, time(_platform.time)
		, input(_platform.input)
		, file_system(_platform.file_system)
		, render(_platform.render)
		, platform(_platform)
		, resources(context, logger)
	{
		time.register_module(context);
		input.register_module(context);
		file_system.register_module(context);
		render.register_module(context);
		platform.register_module(context);

		resources.add_provider(file_system);
	}

	Core::~Core()
	{
		resources.unload_all();
		resources.clear();

		platform.unregister_module(context);
		render.unregister_module(context);
		file_system.unregister_module(context);
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