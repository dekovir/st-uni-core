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
		<text id="position" />
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
		_contex_render.init(renderer);

		// Test UI
#if 1
		_test_doc = std::make_shared<UIDocument>(&_context_logger);
		_test_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_test_view->set_document(_test_doc);
		_test_view->set_title(U"Test UI");
		_test_view->set_size(Vector2f(300, 0));
		_test_view->set_position(Vector2f(50, 50.f));

		UIDocumentParseXML::parse(xml_text, *_test_doc, std::nullopt, &_context_logger);

		_test_position_id = _test_doc->find_by_id("position");
		const auto slider_id = _test_doc->find_by_id("slider");
		const auto group_id = _test_doc->find_by_id("group");
		const auto add_id = _test_doc->find_by_id("add_item");

		if (group_id.has_value() && add_id.has_value())
		{
			_test_doc->set_node_action(add_id.value(), UIActionType::OnClick,
				[this, group_id]
				{
					static int index = 1;
					const auto text = StringBuilder::format("Item {}", index++);

					UINodeOptions options;
					options.attributes[UIAttributeType::Text] = text;
					_test_doc->create_node(UINodeType::Item, options, group_id.value());
				});
		}

		if (slider_id.has_value())
		{
			_test_doc->set_node_action(slider_id.value(), UIActionType::OnChange,
				[this](const Variant& value)
				{
					UC_LOG_DEBUG(logger) << "Slider value changed to " << value;
				});
		}
#endif

		// Inventory UI
#if 1
		_inventory_doc = std::make_shared<UIDocument>(&_context_logger);

		_inventory_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_inventory_view->set_document(_inventory_doc);
		_inventory_view->set_title(U"Inventory");
		_inventory_view->set_position(Vector2f(400, 50.f));

		_inventory = std::make_shared<Inventory>(16);
		_inventory_ui = std::make_shared<InventoryUI>(*_inventory, *_inventory_doc, &_context_logger);

		_item_sprites = resources.load<SpriteList>("items.png"_path, TileSetOptions({ 16, 16 }));
		if (_item_sprites && _item_sprites->size() == 64)
		{
			_inventory->add_item({ U"Dagger", ItemType::Weapon, 100, _item_sprites->get(1) });
			_inventory->add_item({ U"Sword", ItemType::Weapon, 300, _item_sprites->get(2) });

			_inventory->add_item({ U"Potion of Mana", ItemType::Consumable, 75, _item_sprites->get(3) });
			_inventory->add_item({ U"Potion of Health", ItemType::Consumable, 50, _item_sprites->get(4) });
			_inventory->add_item({ U"Potion of Stamina", ItemType::Consumable, 25, _item_sprites->get(5) });

			_inventory->add_item({ U"Brass Plate", ItemType::Armor, 500, _item_sprites->get(8) });
			_inventory->add_item({ U"Steel Plate", ItemType::Armor, 1000, _item_sprites->get(9) });
			_inventory->add_item({ U"Dwarven Plate", ItemType::Armor, 5000, _item_sprites->get(10) });

			_inventory->add_item({ U"Necklace", ItemType::Accessory, 100, _item_sprites->get(14) });

			_inventory->add_item({ U"Staff", ItemType::Weapon, 70, _item_sprites->get(33) });
			_inventory->add_item({ U"Magic orb", ItemType::Accessory, 500, _item_sprites->get(36) });

			_inventory->add_item({ U"Ring of Health", ItemType::Accessory, 1500, _item_sprites->get(38) });
			_inventory->add_item({ U"Ring of Mana", ItemType::Accessory, 1500, _item_sprites->get(39) });

			_inventory->add_item({ U"Crossbow", ItemType::Weapon, 500, _item_sprites->get(48) });
			_inventory->add_item({ U"Spear", ItemType::Weapon, 150, _item_sprites->get(50) });

			_inventory->add_item({ U"Cloak", ItemType::Armor, 100, _item_sprites->get(52) });
		}
		else
		{
			_inventory->add_item({ U"Item 1", ItemType::Accessory, 100, nullptr });
			_inventory->add_item({ U"Item 2", ItemType::Accessory, 125, nullptr });
			_inventory->add_item({ U"Item 3", ItemType::Accessory, 500, nullptr });
		}
#endif
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

		if (_test_doc && _test_view && _test_position_id.has_value())
		{
			auto text = StringBuilder::format("Pos {}, Size {}",
				_test_view->position().cast<Int>(), _test_view->size().cast<Int>());
			_test_doc->set_node_attribute(_test_position_id.value(), UIAttributeType::Text, text);
		}

		// ImGui //////////////////////////////////////////////////////////////////
		_context.frame_begin();

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)
		//static bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
#endif

		if (_test_view)
			_test_view->render();
		if (_inventory_view)
			_inventory_view->render();

		_context.frame_end();
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::DarkSlateGray);

		_context.render();
	}

	UNICORE_MAIN_CORE(MyApp);
}