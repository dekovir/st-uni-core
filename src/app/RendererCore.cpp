#include "unicore/app/RendererCore.hpp"
#include "unicore/platform/Time.hpp"

namespace unicore
{
	RendererCore::RendererCore(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory)
		: DisplayCore(settings)
		, _renderer_logger("[Renderer] ", platform.logger)
		, renderer(renderer_factory(_renderer_logger, display))
	{
		_modules.add(renderer);
	}

	void RendererCore::update()
	{
		DisplayCore::update();

		_fps_counter.update(time.delta());
	}

	void RendererCore::draw()
	{
		if (renderer.begin_frame())
		{
			on_draw();
			renderer.end_frame();
			_fps_counter.frame();
		}
	}

	void RendererCore::frame()
	{
		update();
		draw();
	}
}