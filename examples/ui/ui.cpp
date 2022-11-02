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
		<group value="2">
			<text>Combo</text>
			<combo value="Item 1">
				<text>Text inside combo</text>
				<item>Item 1</item>
				<item>Item 2</item>
				<item>Item 3</item>
				<item>Item 4</item>
			</combo>
		</group>

		<text>Items</text>
		<list id="group">
		</list>
		<button id="add_item">Add item</button>

		<text>Table</text>
		<table value="2">
			<tr>
				<td><button>Cell 0</button></td>
				<td>Cell 1</td>
			</tr>
			<tr>
				<td>Cell 2</td>
				<td>Cell 3</td>
			</tr>
			<tr>
				<td>Cell 4</td>
				<td><item>Cell 5</item></td>
			</tr>
		</table>

		<text>Tree</text>
		<tree value="1">Item 1
			<text>Text 1</text>
			<text>Text 2</text>
			<tree>Item 2
				<text>Text 3</text>
				<text>Text 4</text>
			</tree>
		</tree>
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
					const auto count = group_id.value().get_children_count();
					const auto text = StringBuilder::format("Item {}", count + 1);

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
		_item_sprites = resources.load<SpriteList>("items.png"_path, TileSetOptions({ 16, 16 }));
		if (_item_sprites && _item_sprites->size() == 64)
		{
			_items_db.add({ U"Staff", ItemType::Weapon, 70, _item_sprites->get(33) });
			_items_db.add({ U"Dagger", ItemType::Weapon, 100, _item_sprites->get(1) });
			_items_db.add({ U"Sword", ItemType::Weapon, 300, _item_sprites->get(2) });
			_items_db.add({ U"Crossbow", ItemType::Weapon, 500, _item_sprites->get(48) });
			_items_db.add({ U"Spear", ItemType::Weapon, 150, _item_sprites->get(50) });

			_items_db.add({ U"Potion of Mana", ItemType::Consumable, 75, _item_sprites->get(3) });
			_items_db.add({ U"Potion of Health", ItemType::Consumable, 50, _item_sprites->get(4) });
			_items_db.add({ U"Potion of Stamina", ItemType::Consumable, 25, _item_sprites->get(5) });

			_items_db.add({ U"Cloak", ItemType::Armor, 100, _item_sprites->get(52) });
			_items_db.add({ U"Brass Plate", ItemType::Armor, 500, _item_sprites->get(8) });
			_items_db.add({ U"Steel Plate", ItemType::Armor, 1000, _item_sprites->get(9) });
			_items_db.add({ U"Dwarven Plate", ItemType::Armor, 5000, _item_sprites->get(10) });

			_items_db.add({ U"Necklace", ItemType::Accessory, 100, _item_sprites->get(14) });
			_items_db.add({ U"Magic orb", ItemType::Accessory, 500, _item_sprites->get(36) });
			_items_db.add({ U"Ring of Health", ItemType::Accessory, 1500, _item_sprites->get(38) });
			_items_db.add({ U"Ring of Mana", ItemType::Accessory, 1500, _item_sprites->get(39) });
		}
		else
		{
			_items_db.add({ U"Item 1", ItemType::Accessory, 100, nullptr });
			_items_db.add({ U"Item 2", ItemType::Accessory, 125, nullptr });
			_items_db.add({ U"Item 3", ItemType::Accessory, 500, nullptr });
		}

		_inventory_doc = std::make_shared<UIDocument>(&_context_logger);

		_inventory_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_inventory_view->set_document(_inventory_doc);
		_inventory_view->set_title(U"Inventory");
		_inventory_view->set_position(Vector2f(400, 50.f));

		_inventory = std::make_shared<Inventory>(_items_db, 16);
		_inventory_ui = std::make_shared<InventoryUI>(*_inventory, *_inventory_doc, &_context_logger);

		for (const auto& it : _items_db)
			_inventory->add_item(it.first);

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
		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
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