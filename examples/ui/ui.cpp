#include "ui.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/ui/UIDocumentParseXML.hpp"

namespace unicore
{
	constexpr Vector2i WindowSize = Vector2i(1280, 800);
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyApp::MyApp(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "UI", { false, WindowSize, WindowFlags }))
		, _context_logger("[UI] ", logger)
		, _contex_render(renderer, _context_logger)
		, _context(_contex_render, time, input, _context_logger)
	{
		init_plugins(*this);
	}

	static const auto xml_text = R"(
	<group>
		<text>Sample text</text>
		<group value="2" tooltip="Tooltip text">
			<text>Input</text>
			<input value="quick brown fox" />
		</group>
		<group value="2">
			<text>Slider</text>
			<slider id="slider" value="50.50" max="100">Value %.1f</slider>
		</group>
		<group value="2">
			<text>Toggle</text>
			<toggle value="true" />
		</group>
		<group id="group" />
		<button id="add_item">Add item</button>
	</group>
	)";

	void MyApp::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";

		_contex_render.init(renderer);


		_document = std::make_shared<UIDocument>(&_context_logger);
		_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_view->set_document(_document);

		_inventory = std::make_shared<Inventory>(16);
		_inventory_ui = std::make_shared<InventoryUI>(*_inventory, *_document, &_context_logger);

		_item_sprites = resources.load<SpriteList>("items.png"_path, TileSetOptions({16, 16}));
		if (_item_sprites && _item_sprites->size() == 64)
		{
			_inventory->add_item({ U"Dagger", 100, _item_sprites->get(1)});
			_inventory->add_item({ U"Sword", 300, _item_sprites->get(2)});

			_inventory->add_item({ U"Potion of Mana", 75, _item_sprites->get(3)});
			_inventory->add_item({ U"Potion of Health", 50, _item_sprites->get(4)});
			_inventory->add_item({ U"Potion of Stamina", 25, _item_sprites->get(5)});

			_inventory->add_item({ U"Brass Plate", 500, _item_sprites->get(8)});
			_inventory->add_item({ U"Steel Plate", 1000, _item_sprites->get(9)});
			_inventory->add_item({ U"Dwarven Plate", 5000, _item_sprites->get(10)});

			_inventory->add_item({ U"Necklace", 100, _item_sprites->get(14)});
		}
		else
		{
			_inventory->add_item({ U"Item 1", 100, nullptr });
			_inventory->add_item({ U"Item 2", 125, nullptr });
			_inventory->add_item({ U"Item 3", 500, nullptr });
		}

		UIDocumentParseXML::parse(xml_text, *_document, UINodeIndexInvalid, &_context_logger);

		const auto slider_id = _document->find_index_by_id("slider");
		const auto group_id = _document->find_index_by_id("group");
		const auto add_id = _document->find_index_by_id("add_item");

		if (group_id != UINodeIndexInvalid && add_id != UINodeIndexInvalid)
		{
			_document->set_node_action(add_id, UIActionType::OnClick,
				[this, group_id]()
				{
					static int index = 1;
					const auto text = StringBuilder::format("Item {}", index++);

					UINodeOptions options;
					options.attributes[UIAttributeType::Text] = text;
					_document->create_node(UINodeType::Item, group_id, options);
				});
		}

		if (slider_id != UINodeIndexInvalid)
		{
			_document->set_node_action(slider_id, UIActionType::OnChange,
				[this](const Variant& value)
				{
					UC_LOG_DEBUG(logger) << "Slider value changed to " << value;
				});
		}
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
			mode.window_size = WindowSize;
			mode.window_flags = WindowFlags;
			display.set_mode(mode);
		}
#endif

		// ImGui //////////////////////////////////////////////////////////////////
		_context.frame_begin();

		//static bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);

		_view->render();

		_context.frame_end();
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_context.render();
	}

	UNICORE_MAIN_CORE(MyApp);
}