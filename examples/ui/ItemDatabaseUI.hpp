#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "ItemDatabase.hpp"

namespace unicore
{
	class ItemDatabaseUI
	{
	public:
		ItemDatabaseUI(const Shared<ItemDatabase>& database,
			const Shared<SpriteList>& sprites, UIDocument& document,
			const UINode& parent = UINode::Empty, Logger* logger = nullptr);

		ItemDatabaseUI(const Shared<ItemDatabase>& database,
			const Shared<SpriteList>& sprites,
			UIDocument& document, Logger* logger = nullptr);

	protected:
		Shared<ItemDatabase> _database;
		Shared<SpriteList> _sprites;
		UIDocument& _document;
		Logger* _logger;

		ItemId _selected = ItemId_Invalid;

		UINode _item_group_node;
		UINode _item_template_node;

		UINode _inspector_node;
		UINode _icon_items_node;

		UINode _icon_node;
		UINode _title_node;
		UINode _type_node;
		UINode _price_node;
		UINode _weight_node;

		Dictionary<ItemId, UINode> _item_nodes;
		Dictionary<ItemType, UINode> _type_nodes;

		void on_add_item(ItemId id, const Item& item);

		void set_selected(ItemId id);

		void apply_item(UINode& node, ItemId id);

		void apply_inspector();

		void apply_inspector_icon(const Item& item);
		void apply_inspector_title(const Item& item);
		void apply_inspector_type(const Item& item);
		void apply_inspector_price(const Item& item);
		void apply_inspector_weight(const Item& item);

		void item_set_icon(const Shared<Sprite>& value);
		void item_set_title(StringView32 value);
		void item_set_type(ItemType value);
		void item_set_price(UInt16 value);
		void item_set_weight(UInt16 value);
	};
}