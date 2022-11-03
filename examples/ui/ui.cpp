#include "ui.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/math/Random.hpp"
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
			_test_view->set_position(Vector2f(10, 10));

			_test_position_id = _test_doc->find_by_id("position");
			const auto slider_node = _test_doc->find_by_id("slider");
			const auto group_node = _test_doc->find_by_id("group");
			const auto add_node = _test_doc->find_by_id("add_item");
			const auto combo_node = _test_doc->find_by_id("combo");

			if (!group_node.empty() && !add_node.empty())
			{
				_test_doc->set_node_action(add_node, UIActionType::OnClick,
					[this, group_node]
					{
						const auto count = group_node.get_children_count();
						const auto text = StringBuilder::format("Item {}", count + 1);

						UINodeOptions options;
						options.attributes[UIAttributeType::Text] = text;
						_test_doc->create_node(UINodeType::Item, options, group_node);
					});
			}

			if (!combo_node.empty())
			{
				for (unsigned i = 0; i < 5; i++)
				{
					const auto str = StringBuilder::format("Item {}", i + 1);
					UINodeOptions options;
					options.attributes[UIAttributeType::Text] = str;
					options.attributes[UIAttributeType::Value] = (i == 0);

					auto node = _test_doc->create_node(UINodeType::Item, options, combo_node);
					_test_doc->set_node_action(node, UIActionType::OnClick, 
						[this, combo_node, node]
						{
							for (const auto & child : combo_node.get_children())
							{
								if (child.type() == UINodeType::Item)
									_test_doc->set_node_attribute(child, UIAttributeType::Value, false);
							}

							_test_doc->set_node_attribute(node, UIAttributeType::Value, true);
							_test_doc->set_node_attribute(combo_node, UIAttributeType::Value, node.text());
						});
				}
				_test_doc->set_node_attribute(combo_node, UIAttributeType::Value, "Item 1");
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
				_items_db.add(Item::make_weapon(U"Staff", 50, 1500, { 5, 10 }, _item_sprites->get(33)));
				_items_db.add(Item::make_weapon(U"Dagger", 150, 1000, { 2, 5 }, _item_sprites->get(1)));
				_items_db.add(Item::make_weapon(U"Sword", 350, 2000, { 7, 14 }, _item_sprites->get(2)));
				_items_db.add(Item::make_weapon(U"Crossbow", 500, 4000, { 5, 12 }, _item_sprites->get(48)));
				_items_db.add(Item::make_weapon(U"Spear", 150, 5000, { 8,  16 }, _item_sprites->get(50)));

				_items_db.add(Item::make_shield(U"Shield", 250, 1500, 5, _item_sprites->get(43)));

				_items_db.add(Item::make_consumable(U"Potion of Mana", 75, 150, _item_sprites->get(3)));
				_items_db.add(Item::make_consumable(U"Potion of Health", 50, 150, _item_sprites->get(4)));
				_items_db.add(Item::make_consumable(U"Potion of Stamina", 25, 150, _item_sprites->get(5)));

				_items_db.add(Item::make_armor(U"Cloak", 100, 300, 1, _item_sprites->get(52)));
				_items_db.add(Item::make_armor(U"Brass Plate", 500, 15000, 15, _item_sprites->get(8)));
				_items_db.add(Item::make_armor(U"Steel Plate", 1000, 13000, 25, _item_sprites->get(9)));
				_items_db.add(Item::make_armor(U"Dwarven Plate", 5000, 14000, 40, _item_sprites->get(10)));

				_items_db.add(Item::make_accessory(U"Necklace", 100, 300, _item_sprites->get(14)));
				_items_db.add(Item::make_accessory(U"Magic orb", 500, 1100, _item_sprites->get(36)));
				_items_db.add(Item::make_accessory(U"Ring of Health", 1500, 100, _item_sprites->get(38)));
				_items_db.add(Item::make_accessory(U"Ring of Mana", 1500, 100, _item_sprites->get(39)));
			}
			else
			{
				DefaultRandom random;
				for (unsigned i = 0; i < 5; i++)
				{
					Item item;
					item.type = ItemType::Accessory;
					item.price = random.range(100, 900);
					item.weight = random.range(100, 15000);
					item.title = StringBuilder::format(U"Item {}", i + 1);
					_items_db.add(item);
				}
			}

			_inventory_view = std::make_shared<UIViewImGui>(_context, _context_logger);
			_inventory_view->set_document(_inventory_doc);
			_inventory_view->set_title(U"Inventory");
			_inventory_view->set_position(Vector2f(320, 10));

			_inventory = std::make_shared<Inventory>(_items_db);
			_inventory_ui = std::make_shared<InventoryUI>(
				*_inventory, *_inventory_doc, &_context_logger);

			for (const auto& it : _items_db)
			{
				const auto item = _inventory->database().get(it.first);
				if (item->is_stackable())
					_inventory->add_item(it.first, 10);
				else if (item->has_status())
					_inventory->add_item(it.first, 200);
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
		static bool show_demo_window = false;
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