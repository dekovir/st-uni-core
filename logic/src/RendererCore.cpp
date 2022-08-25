#include "unicore/app/RendererCore.hpp"
#include "unicore/Time.hpp"
#include "unicore/plugins/RendererPlugin.hpp"

namespace unicore
{
	RendererCore::RendererCore(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory)
		: DisplayCore(settings)
		, _renderer_logger("[Renderer] ", platform.logger)
		, renderer(renderer_factory(_renderer_logger, display))
	{
		_modules.add(renderer);

		create_plugin<RendererPlugin>();
	}

	void RendererCore::update()
	{
		DisplayCore::update();

		_fps_time += time.delta();
		if (_fps_time >= TimeSpanConst::OneSecond)
		{
			_fps_current = _fps_counter;

			_fps_counter = 0;
			_fps_time -= TimeSpanConst::OneSecond;
		}
	}

	void RendererCore::draw()
	{
		_fps_counter++;
		if (renderer.begin_scene())
		{
			on_draw();
			renderer.end_scene();
		}
	}

	void RendererCore::frame()
	{
		update();
		draw();
	}
}