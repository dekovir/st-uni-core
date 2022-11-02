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

	void MyApp::on_init()
	{
		_contex_render.init(renderer);

		// Test UI
#if 1
		_test_doc = resources.load<UIDocument>("ui.xml"_path, LoggerOption{ _context_logger });
		if (_test_doc)
		{
			_test_view = std::make_shared<UIViewImGui>(_context, _context_logger);
			_test_view->set_document(_test_doc);
			_test_view->set_title(U"Test UI");
			_test_view->set_size(Vector2f(300, 0));
			_test_view->set_position(Vector2f(50, 50.f));

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
		}
#endif

		// Inventory UI
#if 1
		_inventory_doc = resources.load<UIDocument>("inventory.xml"_path, LoggerOption{ _context_logger });
		if (_inventory_doc)
		{
			_item_sprites = resources.load<SpriteList>("items.png"_path, TileSetOptions({ 16, 16 }));
			if (_item_sprites && _item_sprites->size() == 64)
			{
				_items_db.add({ U"Staff", ItemType::Weapon, 50, 1500, _item_sprites->get(33) });
				_items_db.add({ U"Dagger", ItemType::Weapon, 150, 1000, _item_sprites->get(1) });
				_items_db.add({ U"Sword", ItemType::Weapon, 350, 2000, _item_sprites->get(2) });
				_items_db.add({ U"Crossbow", ItemType::Weapon, 500, 4000, _item_sprites->get(48) });
				_items_db.add({ U"Spear", ItemType::Weapon, 150, 5000, _item_sprites->get(50) });

				_items_db.add({ U"Shield", ItemType::Shield, 250, 1500, _item_sprites->get(43) });

				_items_db.add({ U"Potion of Mana", ItemType::Consumable, 75, 150, _item_sprites->get(3) });
				_items_db.add({ U"Potion of Health", ItemType::Consumable, 50, 150, _item_sprites->get(4) });
				_items_db.add({ U"Potion of Stamina", ItemType::Consumable, 25, 150, _item_sprites->get(5) });

				_items_db.add({ U"Cloak", ItemType::Armor, 100, 300, _item_sprites->get(52) });
				_items_db.add({ U"Brass Plate", ItemType::Armor, 500, 15000, _item_sprites->get(8) });
				_items_db.add({ U"Steel Plate", ItemType::Armor, 1000, 13000, _item_sprites->get(9) });
				_items_db.add({ U"Dwarven Plate", ItemType::Armor, 5000, 14000, _item_sprites->get(10) });

				_items_db.add({ U"Necklace", ItemType::Accessory, 100, 300, _item_sprites->get(14) });
				_items_db.add({ U"Magic orb", ItemType::Accessory, 500, 1100, _item_sprites->get(36) });
				_items_db.add({ U"Ring of Health", ItemType::Accessory, 1500, 100, _item_sprites->get(38) });
				_items_db.add({ U"Ring of Mana", ItemType::Accessory, 1500, 100, _item_sprites->get(39) });
			}
			else
			{
				_items_db.add({ U"Item 1", ItemType::Accessory, 100, 100, nullptr });
				_items_db.add({ U"Item 2", ItemType::Accessory, 125, 100, nullptr });
				_items_db.add({ U"Item 3", ItemType::Accessory, 500, 100, nullptr });
			}

			_inventory_view = std::make_shared<UIViewImGui>(_context, _context_logger);
			_inventory_view->set_document(_inventory_doc);
			_inventory_view->set_title(U"Inventory");
			_inventory_view->set_position(Vector2f(400, 50.f));

			_inventory = std::make_shared<Inventory>(_items_db);
			_inventory_ui = std::make_shared<InventoryUI>(
				*_inventory, *_inventory_doc, &_context_logger);

			for (const auto& it : _items_db)
			{
				const auto item = _inventory->database().get(it.first);
				if (item->is_stackable())
					_inventory->add_item(it.first, 10);
				else _inventory->add_item(it.first);
			}
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