#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "ItemDatabase.hpp"

namespace unicore
{
	class ItemDatabaseUI
	{
	public:
		ItemDatabaseUI(ItemDatabase& database, UIDocument& document,
			const UINode& parent = UINode::Empty, Logger* logger = nullptr);

		ItemDatabaseUI(ItemDatabase& database,
			UIDocument& document, Logger* logger = nullptr);

	protected:
		ItemDatabase& _database;
		UIDocument& _document;
		Logger* _logger;
		ItemId _selected = ItemId_Invalid;

		UINode _items_list_node;
		UINode _inspector_node;

		UINode _title_node;
		UINode _type_node;
		UINode _price_node;
		UINode _weight_node;

		Dictionary<ItemId, UINode> _item_nodes;

		void on_add_item(ItemId id, const Item& item);

		void set_selected(ItemId id);

		void apply_item(UINode& node, ItemId id);
		void apply_inspector();
	};
}