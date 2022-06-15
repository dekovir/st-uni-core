#include "unicore/Core.hpp"

namespace unicore
{
	Core::Core(Platform& _platform)
		: logger(_platform.logger)
		, time(_platform.time)
		, input(_platform.input)
		, render(_platform.render)
		, file_system(_platform.file_system)
		, platform(_platform)
	{
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