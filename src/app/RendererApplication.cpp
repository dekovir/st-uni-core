#include "unicore/app/RendererApplication.hpp"
#include "unicore/platform/Time.hpp"

namespace unicore
{
	RendererApplication::RendererApplication(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory)
		: DisplayApplication(settings)
		, _renderer_logger("[Renderer] ", platform.logger)
		, renderer(renderer_factory(_renderer_logger, display))
	{
		_modules.add(renderer);
	}

	void RendererApplication::update()
	{
		DisplayApplication::update();

		_fps_counter.update(time.delta());
	}

	void RendererApplication::draw()
	{
		if (renderer.begin_frame())
		{
			on_draw();
			renderer.end_frame();
			_fps_counter.frame();
		}
	}

	void RendererApplication::frame()
	{
		update();
		draw();
	}
}