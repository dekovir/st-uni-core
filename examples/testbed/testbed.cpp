#include "testbed.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/io/FileProvider.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/io/FileLoader.hpp"

namespace unicore
{
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyApp::MyApp(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "Testbed"))
	{
		UC_LOG_INFO(logger) << "Starting";

		init_plugins(*this);
	}

	void MyApp::on_init()
	{
#if 1
		_archive = resources.load<ReadFileProvider>("negative.7z"_path);
		if (_archive)
			resources.add_loader(std::make_shared<ReadFileLoader>(*_archive));
#endif

		//_font = resources.create<Font>(EmptyResourceOptions{});
		//_font = resources.load<Font>(L"font_004.fnt"_path);
		_font = resources.load<Font>("ubuntu.regular.ttf"_path);

		set_example(0);
	}

	void MyApp::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.looper.quit();

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
		const auto& example_info = examples[_example_index];

		const auto title_str = StringBuilder::format(U"Example: {}", example_info.title);
		const auto fps_str = StringBuilder::format(U"FPS: {}", fps());
		const auto draw_str = StringBuilder::format(U"Draw: {}", _draw_calls);
		const auto screen_str = StringBuilder::format(U"Screen: {}", screen_size);

		_lines.clear();
		_example->get_text(_lines);

		if (_font)
		{
			const float height = _font->get_height();

			_sprite_batch
				.clear()
				.print(_font, { 0, 0 }, fps_str)
				.print(_font, { 0, height * 1 }, draw_str)
				.print(_font, { 0, height * 2 }, screen_str);

			_sprite_batch
				.print(_font, { 250, 0 }, title_str);

			for (unsigned i = 0; i < _lines.size(); i++)
			{
				_sprite_batch
					.print(_font, { 250, height * (i + 1) }, _lines[i]);
			}

			_sprite_batch.flush();
		}
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		if (_example)
			_example->draw();

		_sprite_batch.render(renderer);

		_draw_calls = renderer.draw_calls();
	}

	void MyApp::set_example(int index)
	{
		if (_example_index == index) return;

		auto& info = ExampleCatalog::get_all()[index];

		const auto example = info.factory({ logger, _random, time, input, renderer, platform, _font });
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
		//resources.dump_used();
	}

	UNICORE_MAIN_CORE(MyApp);
}