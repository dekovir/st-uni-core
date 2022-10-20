#include "raycast.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/platform/Input.hpp"

namespace unicore
{
	constexpr Vector2i WindowSize = Vector2i(800, 600);
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyCore::MyCore(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "Raycast", { false, WindowSize, WindowFlags }))
	{
		init_plugins(*this);
	}

	void MyCore::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";
	}

	void MyCore::on_update()
	{
		const auto screen_size = renderer.screen_size();

#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.looper.quit();

		if (
			input.keyboard().down_changed(KeyCode::Enter) &&
			input.keyboard().mods(KeyModCombine::Alt))
		{
			auto mode = display.get_mode();
			mode.fullscreen = !mode.fullscreen;
			mode.window_size = WindowSize;
			mode.window_flags = WindowFlags;
			display.set_mode(mode);
		}
#endif
	}

	void MyCore::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_graphics.render(renderer);

		_sprite_batch.render(renderer);
	}

	UNICORE_MAIN_CORE(MyCore);
}