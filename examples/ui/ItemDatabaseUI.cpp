#include "ItemDatabaseUI.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	ItemDatabaseUI::ItemDatabaseUI(const Shared<ItemDatabase>& database,
		const Shared<SpriteList>& sprites, UIDocument& document,
		const UINode& parent, Logger* logger)
		: _database(database)
		, _sprites(sprites)
		, _document(document)
		, _logger(logger)
	{
		_item_group_node = document.find_by_name("item_group", parent);
		_item_template_node = document.find_by_name("item_template", parent);
		_inspector_node = document.find_by_name("item_edit", parent);

		_icon_items_node = document.find_by_name("icon_popup", _inspector_node);
		const auto icon_list_node = document.find_by_name("icon_list", _inspector_node);

		_icon_node = document.find_by_name("icon", _inspector_node);
		_title_node = document.find_by_name("title", _inspector_node);
		_type_node = document.find_by_name("type", _inspector_node);
		_price_node = document.find_by_name("price", _inspector_node);
		_weight_node = document.find_by_name("weight", _inspector_node);

		_document.set_node_action(_icon_node, UIActionType::OnClick,
			[this] { _document.set_node_attribute(_icon_items_node, UIAttribute::Value, true); });

		if (!icon_list_node.empty() && _sprites && !_sprites->empty())
		{
			for (unsigned i = 0; i < _sprites->size(); i++)
			{
				auto spr = _sprites->get(i);

				UINodeOptions options;
				options.attributes[UIAttribute::Value] = spr;
				//options.attributes[UIAttribute::Text] = StringBuilder::format("{}", i + 1);
				options.attributes[UIAttribute::Type] = UIInputType::Image;
				options.attributes[UIAttribute::Width] = 32;
				options.attributes[UIAttribute::Height] = 32;
				options.actions[UIActionType::OnClick] =
					[this, spr] { item_set_icon(spr); };

				_document.create_node(UINodeTag::Input, options, icon_list_node);
			}
		}

		_database->on_add() += [&](auto id, auto& item) {on_add_item(id, item); };

		_document.set_node_action(_title_node, UIActionType::OnChange,
			[this](const Variant& value) { item_set_title(value.get_string32()); });

		_document.set_node_action(_price_node, UIActionType::OnChange,
			[this](const Variant& value) { item_set_price(value.get_int_type<UInt16>()); });

		_document.set_node_action(_weight_node, UIActionType::OnChange,
			[this](const Variant& value) { item_set_weight(value.get_int_type<UInt16>()); });

		if (!_type_node.empty())
		{
			for (const auto& it : Item::TypeNames)
			{
				const auto store_type = it.first;

				UINodeOptions options;
				options.attributes[UIAttribute::Text] = it.second;
				options.actions[UIActionType::OnClick] =
					[this, store_type] { item_set_type(store_type); };
				_type_nodes[store_type] = _document.create_node(
					UINodeTag::Item, options, _type_node);
			}
		}

		if (!_item_group_node.empty())
		{
			for (const auto& it : *_database)
				on_add_item(it.first, *it.second);
		}

		if (_database->begin() != _database->end())
			set_selected(_database->begin()->first);
		else apply_inspector();
	}

	ItemDatabaseUI::ItemDatabaseUI(const Shared<ItemDatabase>& database,
		const Shared<SpriteList>& sprites, UIDocument& document, Logger* logger)
		: ItemDatabaseUI(database, sprites, document, UINode::Empty, logger)
	{
	}

	void ItemDatabaseUI::on_add_item(ItemId id, const Item& item)
	{
		if (_item_group_node.empty() || _item_template_node.empty()) return;

		UINodeOptions options;

		if (auto node = _document.duplicate(_item_template_node, _item_group_node); !node.empty())
		{
			_item_nodes[id] = node;

			if (const auto find = node.find_by_name("item_name"); !find.empty())
			{
				const auto stored_id = id;
				_document.set_node_action(find, UIActionType::OnClick,
					[this, stored_id] { set_selected(stored_id); });
			}
			_document.set_node_visible(node, true);

			apply_item(node, id);
		}
	}

	void ItemDatabaseUI::set_selected(ItemId id)
	{
		if (_selected == id) return;

		//UC_LOG_DEBUG(_logger) << "Selected set to " << id.value;

		_selected = id;
		for (const auto& it : _item_nodes)
		{
			if (const auto find = it.second.find_by_name("item_name"); !find.empty())
				_document.set_node_attribute(find, UIAttribute::Value, it.first == _selected);
		}
		apply_inspector();
	}

	void ItemDatabaseUI::apply_item(UINode& node, ItemId id)
	{
		const auto item = _database->get(id);
		if (!item) return;

		if (const auto find = node.find_by_name("item_id"); !find.empty())
			_document.set_node_attribute(find, UIAttribute::Text, id.value);

		if (const auto find = node.find_by_name("item_icon"); !find.empty())
			_document.set_node_attribute(find, UIAttribute::Value, item->sprite);

		if (const auto find = node.find_by_name("item_name"); !find.empty())
			_document.set_node_attribute(find, UIAttribute::Text, item->title);
	}

	void ItemDatabaseUI::apply_inspector()
	{
		const auto item = _database->get(_selected);
		if (!item)
		{
			_document.set_node_visible(_inspector_node, false);
			return;
		}

		_document.set_node_visible(_inspector_node, true);

		apply_inspector_icon(*item);
		apply_inspector_title(*item);
		apply_inspector_type(*item);
		apply_inspector_price(*item);
		apply_inspector_weight(*item);
	}

	void ItemDatabaseUI::apply_inspector_icon(const Item& item)
	{
		_document.set_node_attribute(_icon_node,
			UIAttribute::Value, item.sprite);
	}

	void ItemDatabaseUI::apply_inspector_title(const Item& item)
	{
		_document.set_node_attribute(_title_node,
			UIAttribute::Value, item.title);
	}

	void ItemDatabaseUI::apply_inspector_type(const Item& item)
	{
		_document.set_node_attribute(_type_node,
			UIAttribute::Value, Item::type_to_string(item.item_type));

		for (const auto& it : _type_nodes)
			_document.set_node_attribute(it.second,
				UIAttribute::Value, it.first == item.item_type);
	}

	void ItemDatabaseUI::apply_inspector_price(const Item& item)
	{
		_document.set_node_attribute(_price_node,
			UIAttribute::Value, item.price);
	}

	void ItemDatabaseUI::apply_inspector_weight(const Item& item)
	{
		_document.set_node_attribute(_weight_node,
			UIAttribute::Value, item.weight);
	}

	void ItemDatabaseUI::item_set_icon(const Shared<Sprite>& value)
	{
		if (const auto it = _database->items().find(_selected); it != _database->items().end())
		{
			it->second->sprite = value;
			apply_inspector_icon(*it->second);
		}

		if (const auto it = _item_nodes.find(_selected); it != _item_nodes.end())
			apply_item(it->second, it->first);
	}

	void ItemDatabaseUI::item_set_title(StringView32 value)
	{
		if (const auto it = _database->items().find(_selected); it != _database->items().end())
		{
			it->second->title = value;
			apply_inspector_title(*it->second);
		}

		if (const auto it = _item_nodes.find(_selected); it != _item_nodes.end())
			apply_item(it->second, it->first);
	}

	void ItemDatabaseUI::item_set_type(ItemType value)
	{
		const auto it = _database->items().find(_selected);
		if (it != _database->items().end())
		{
			it->second->item_type = value;
			apply_inspector_type(*it->second);
		}
	}

	void ItemDatabaseUI::item_set_price(UInt16 value)
	{
		const auto it = _database->items().find(_selected);
		if (it != _database->items().end())
		{
			it->second->price = value;
			apply_inspector_price(*it->second);
		}
	}

	void ItemDatabaseUI::item_set_weight(UInt16 value)
	{
		const auto it = _database->items().find(_selected);
		if (it != _database->items().end())
		{
			it->second->weight = value;
			apply_inspector_weight(*it->second);
		}
	}
}