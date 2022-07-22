#include "testbed.hpp"
#include "UnicoreMain.h"
#include "unicore/Input.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Font.hpp"
#include "unicore/xml/XMLPlugin.hpp"
#include "unicore/fnt/FNTPlugin.hpp"

namespace unicore
{
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyCore::MyCore(const CoreSettings& settings)
		: SDLCore(create_settings(settings, "Testbed"))
	{
		UC_LOG_INFO(logger) << "Starting";

		add_plugin<XMLPlugin>();
		add_plugin<FNTPlugin>();

		_font = resources.load<Font>(L"font_004.fnt"_path);

		set_example(0);
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();

		if (
			input.keyboard().down_change(KeyCode::Enter) &&
			input.keyboard().mods(KeyModCombine::Alt))
		{
			auto mode = display.get_mode();
			mode.fullscreen = !mode.fullscreen;
			mode.window_size = display.get_maximum_size();
			mode.window_flags = WindowFlags;
			display.set_mode(mode);
		}
#endif

		auto& examples = ExampleCatalog::get_all();
		if (input.keyboard().down_change(KeyCode::BracketRight))
			set_example((_example_index + 1) % examples.size());
		if (input.keyboard().down_change(KeyCode::BracketLeft))
			set_example((_example_index + examples.size() - 1) % examples.size());

		if (_example)
			_example->update();

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		const auto screen_size = renderer.screen_size();

		const String title_str = "Example: " + examples[_example_index].title;
		const String fps_str = "FPS: " + std::to_string(fps());
		const String draw_str = "Draw: " + std::to_string(_draw_calls);
		const String screen_str = "Screen: " + std::to_string(screen_size.x) + "x" + std::to_string(screen_size.y);

		_sprite_batch.begin();

		_sprite_batch.print(_font, { 0, 0 }, fps_str);
		_sprite_batch.print(_font, { 200, 0 }, title_str);
		_sprite_batch.print(_font, { 0, 20 }, draw_str);
		_sprite_batch.print(_font, { 0, 40 }, screen_str);

		_sprite_batch.end();
	}

	void MyCore::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		if (_example)
			_example->draw();

		_sprite_batch.render(renderer);

		_draw_calls = renderer.draw_calls();
	}

	void MyCore::set_example(int index)
	{
		if (_example_index == index) return;

		auto& info = ExampleCatalog::get_all()[index];

		const auto example = info.factory({ logger, _random, time, input, renderer });
		if (!example->load(resources))
			return;

		_example_index = index;
		resources.unload_unused();
		_example = example;
	}

	UNICORE_MAIN_CORE(MyCore);
}