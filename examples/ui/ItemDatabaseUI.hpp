#pragma once
#include "unicore/remoteui/Document.hpp"
#include "ItemDatabase.hpp"

namespace unicore
{
	class ItemDatabaseUI
	{
	public:
		ItemDatabaseUI(const Shared<ItemDatabase>& database,
			const Shared<SpriteList>& sprites, remoteui::Document& document,
			const remoteui::Element& parent = remoteui::Element::Empty,
			Logger* logger = nullptr);

		ItemDatabaseUI(const Shared<ItemDatabase>& database,
			const Shared<SpriteList>& sprites,
			remoteui::Document& document, Logger* logger = nullptr);

	protected:
		Shared<ItemDatabase> _database;
		Shared<SpriteList> _sprites;
		remoteui::Document& _document;
		Logger* _logger;

		ItemId _selected = ItemId_Invalid;

		remoteui::Element _item_group_node;
		remoteui::Element _item_template_node;

		remoteui::Element _inspector_node;
		remoteui::Element _icon_items_node;

		remoteui::Element _icon_node;
		remoteui::Element _title_node;
		remoteui::Element _type_node;
		remoteui::Element _price_node;
		remoteui::Element _weight_node;

		Dictionary<ItemId, remoteui::Element> _item_nodes;
		Dictionary<ItemType, remoteui::Element> _type_nodes;

		void on_add_item(ItemId id, const Item& item);

		void set_selected(ItemId id);

		void apply_item(remoteui::Element& node, ItemId id);

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