#include "ItemDatabaseUI.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	ItemDatabaseUI::ItemDatabaseUI(ItemDatabase& database,
		UIDocument& document, const UINode& parent, Logger* logger)
		: _database(database)
		, _document(document)
		, _logger(logger)
	{
		_items_list_node = document.find_by_name("items_list", parent);
		_inspector_node = document.find_by_name("item_edit", parent);

		_title_node = document.find_by_name("title", _inspector_node);
		_type_node = document.find_by_name("type", _inspector_node);
		_price_node = document.find_by_name("price", _inspector_node);
		_weight_node = document.find_by_name("weigth", _inspector_node);

		_database.on_add() += [&](auto id, auto& item) {on_add_item(id, item); };

		if (!_type_node.empty())
		{
			for (const auto& it : Item::TypeNames)
			{
				UINodeOptions options;
				options.attributes[UIAttributeType::Text] = it.second;
				_document.create_node(UINodeType::Item, options, _type_node);
			}
		}

		if (!_items_list_node.empty())
		{
			for (const auto& it : _database)
				on_add_item(it.first, *it.second);
		}

		if (_database.begin() != _database.end())
			set_selected(_database.begin()->first);
		else apply_inspector();
	}

	ItemDatabaseUI::ItemDatabaseUI(ItemDatabase& database,
		UIDocument& document, Logger* logger)
		: ItemDatabaseUI(database, document, UINode::Empty, logger)
	{
	}

	void ItemDatabaseUI::on_add_item(ItemId id, const Item& item)
	{
		if (_items_list_node.empty()) return;

		UINodeOptions options;

		const auto stored_id = id;
		options.actions[UIActionType::OnClick] =
			[this, stored_id] { set_selected(stored_id); };

		if (auto node = _document.create_node(
			UINodeType::Item, options, _items_list_node); !node.empty())
		{
			_item_nodes[id] = node;
			apply_item(node, id);
		}
	}

	void ItemDatabaseUI::set_selected(ItemId id)
	{
		if (_selected == id) return;

		//UC_LOG_DEBUG(_logger) << "Selected set to " << id.value;

		_selected = id;
		for (const auto& it : _item_nodes)
			_document.set_node_attribute(it.second, UIAttributeType::Value, it.first == _selected);
		apply_inspector();
	}

	void ItemDatabaseUI::apply_item(UINode& node, ItemId id)
	{
		const auto item = _database.get(id);
		if (!item) return;

		const auto str = StringBuilder::format("{}: {}", id.value, item->title);
		_document.set_node_attribute(node, UIAttributeType::Text, str);
	}

	void ItemDatabaseUI::apply_inspector()
	{
		const auto item = _database.get(_selected);
		if (!item)
		{
			_document.set_node_visible(_inspector_node, false);
			return;
		}

		_document.set_node_visible(_inspector_node, true);

		_document.set_node_attribute(_title_node, UIAttributeType::Value, item->title);
		_document.set_node_attribute(_type_node, UIAttributeType::Value, Item::type_to_string(item->item_type));
		_document.set_node_attribute(_price_node, UIAttributeType::Value, item->price);
		_document.set_node_attribute(_weight_node, UIAttributeType::Value, item->weight);
	}
}