#include "testbed.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/io/FileProvider.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/io/FileLoader.hpp"
#include "unicore/remoteui/DocumentParseXML.hpp"

namespace unicore
{
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	static const auto xml = R"(
	<group>
		<text>Examples</text>
		<list name="group" w="-1" h="-1" />
		<group name="template" visible="0">
			<item name="name">Title</item>
		</group>
	</group>
	)";

	MyApp::MyApp(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "Testbed"))
		, _ui_logger("[UI] ", logger)
		, _ui_render(renderer, _ui_logger)
		, _ui_context(_ui_render, time, input, _ui_logger)
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

		_ui_render.init(renderer);

		_ui_document = std::make_shared<remoteui::Document>(&_ui_logger);
		_ui_view = std::make_shared<remoteui::ViewImGui>(_ui_context, _ui_logger);
		_ui_view->set_document(_ui_document);
		_ui_view->set_size({ 300, 400 });
		_ui_view->flags().set(remoteui::ViewImGui::Flag::NoTiltebar);
		_ui_view->flags().set(remoteui::ViewImGui::Flag::NoMove);
		_ui_view->flags().set(remoteui::ViewImGui::Flag::NoResize);

		if (remoteui::DocumentParseXML::parse(xml, *_ui_document, remoteui::Element::Empty, &_ui_logger))
		{
			const auto group_node = _ui_document->find_by_name("group");
			const auto template_node = _ui_document->find_by_name("template");
			if (!group_node.empty() && !template_node.empty())
			{
				const auto& example_infos = ExampleCatalog::get_all();
				for (unsigned i = 0; i < example_infos.size(); i++)
				{
					const auto node = _ui_document->duplicate(template_node, group_node);
					if (!node.empty())
					{
						_ui_document->set_node_attribute(node, remoteui::Attribute::Hidden, false);

						auto& info = example_infos[i];

						const auto index = static_cast<int>(i);

						if (auto find = node.find_by_name("name"); find.valid())
						{
							_ui_document->set_node_attribute(find, remoteui::Attribute::Text, info.title);
							_ui_document->subscribe_node(find, remoteui::UIActionType::OnClick,
								[this, index] { set_example(index); });
						}
					}
				}
			}
		}

		//set_example(0);
		//set_example(ExampleCatalog::get_all().size() - 1);
	}

	void MyApp::on_update()
	{
		if (input.keyboard().down_changed(KeyCode::Escape))
		{
			if (_example) set_example(-1);
			else platform.looper.quit();
		}

#if !defined(UNICORE_PLATFORM_WEB)
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

		const auto screen_size = renderer.screen_size();
		auto& examples = ExampleCatalog::get_all();

		if (input.keyboard().down_changed(KeyCode::BracketRight))
			set_example((_example_index + 1) % examples.size());
		if (input.keyboard().down_changed(KeyCode::BracketLeft))
			set_example((_example_index + examples.size() - 1) % examples.size());

		_sprite_batch.clear();

		_ui_context.frame_begin();

		const auto fps_str = StringBuilder::format(U"FPS: {}", fps());
		const auto draw_str = StringBuilder::format(U"Draw: {}", _draw_calls);
		const auto screen_str = StringBuilder::format(U"Screen: {}", screen_size);

		if (_font)
		{
			const float height = _font->get_height();

			_sprite_batch
				.print(_font, { 0, 0 }, fps_str)
				.print(_font, { 0, height * 1 }, draw_str)
				.print(_font, { 0, height * 2 }, screen_str);
		}

		// EXAMPLE ////////////////////////////////////////////////////////////
		if (_example)
		{
			_example->update();

			const auto& example_info = examples[_example_index];
			const auto title_str = StringBuilder::format(U"Example: {}", example_info.title);

			_lines.clear();
			_example->get_text(_lines);

			String32 comment;
			_example->get_comment(comment);

			if (_font)
			{
				const float height = _font->get_height();

				_sprite_batch
					.print(_font, { 250, 0 }, title_str);

				for (unsigned i = 0; i < _lines.size(); i++)
				{
					_sprite_batch
						.print(_font, { 250, height * (i + 1) }, _lines[i]);
				}

				if (!comment.empty())
					_sprite_batch.print(_font, { 0, screen_size.y - height }, comment);
			}
		}
		else if (_ui_view)
		{
			_ui_view->set_position((screen_size.cast<Float>() - _ui_view->size()) / 2.f);
			_ui_view->render();
		}

		_sprite_batch.flush();
		_ui_context.frame_end();
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		if (_example)
			_example->draw();

		_sprite_batch.render(renderer);

		_ui_context.render();

		_draw_calls = renderer.draw_calls();
	}

	void MyApp::on_drop_file(const Path& path)
	{
		if (_example)
			_example->on_drop_file(path);
	}

	void MyApp::set_example(int index)
	{
		if (_example_index == index) return;

		if (index >= 0)
		{
			auto& info = ExampleCatalog::get_all()[index];

			auto example = info.factory({ logger, _random, time, input, renderer, platform, _font, _ui_context });
			if (!example)
			{
				UC_LOG_ERROR(logger) << "Failed to create example " << info.title << ":" << index;
				return;
			}

			_example_index = index;
			_example = std::move(example);
			UC_LOG_INFO(logger) << "Set example " << index << ": " << info.title;

			_example->load(resources);
		}
		else
		{
			_example_index = -1;
			_example = nullptr;
		}

		resources.unload_unused();
		//resources.dump_used();
	}

	UNICORE_MAIN_CORE(MyApp);
}