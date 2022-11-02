#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class InventoryUI
	{
	public:
		InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger = nullptr);

	protected:
		Inventory& _inventory;
		UIDocument& _document;
		Logger* _logger;

		Optional<UINode> _money_text;
		Optional<UINode> _items_group;
		Optional<UINode> _item_template;
		Optional<UINode> _item_tooltip;

		void on_change_money(UInt16 value);
		void on_add_item(unsigned index, ItemId id);
		void on_remove_item(unsigned index, ItemId id);

		void apply_money(UInt16 value);
		void apply_item(const UINode& node, ItemId id);
		void apply_tooltip(ItemId id);

		static StringView type_to_string(ItemType type);
	};
}