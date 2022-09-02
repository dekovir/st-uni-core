#include "testbed.hpp"
#include "UnicoreMain.h"
#include "unicore/Input.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Font.hpp"
#include "unicore/xml/XMLPlugin.hpp"
#include "unicore/fnt/FNTPlugin.hpp"
#include "unicore/imgui/ImGuiPlugin.hpp"

namespace unicore
{
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyCore::MyCore(const CoreSettings& settings)
		: SDLCore(create_settings(settings, "Testbed"))
	{
		UC_LOG_INFO(logger) << "Starting";

		create_plugin<XMLPlugin>();
		create_plugin<FNTPlugin>();
		create_plugin<ImGuiPlugin>();
	}

	void MyCore::on_init()
	{
		_font = resources.load<Font>(L"font_004.fnt"_path);

		set_example(0);
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();

		if (
			input.keyboard().down_changed(KeyCode::Enter) &&
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
		if (input.keyboard().down_changed(KeyCode::BracketRight))
			set_example((_example_index + 1) % examples.size());
		if (input.keyboard().down_changed(KeyCode::BracketLeft))
			set_example((_example_index + examples.size() - 1) % examples.size());

		if (_example)
			_example->update();

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		const auto screen_size = renderer.screen_size();

		const auto title_str = StringBuilder::format("Example: {}", examples[_example_index].title);
		const auto fps_str = StringBuilder::format("FPS: {}", fps());
		const auto draw_str = StringBuilder::format("Draw: {}", _draw_calls);
		const auto screen_str = StringBuilder::format("Screen: {}", screen_size);

		_sprite_batch
			.clear()
			.print(_font, { 0, 0 }, fps_str)
			.print(_font, { 200, 0 }, title_str)
			.print(_font, { 0, 20 }, draw_str)
			.print(_font, { 0, 40 }, screen_str)
			.flush();
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

		const auto example = info.factory({ logger, _random, time, input, renderer, platform });
		if (!example)
		{
			UC_LOG_ERROR(logger) << "Failed to create example " << info.title << ":" << index;
			return;
		}

		_example_index = index;
		_example = example;
		UC_LOG_INFO(logger) << "Set example " << index << ": " << info.title;

		example->load(resources);
		resources.unload_unused();
	}

	UNICORE_MAIN_CORE(MyCore);
}