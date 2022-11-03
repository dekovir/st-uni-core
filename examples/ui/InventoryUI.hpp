#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class InventoryUI
	{
	public:
		InventoryUI(Inventory& inventory, UIDocument& document,
			const Optional<UINode>& parent, Logger* logger = nullptr);
		InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger = nullptr);

	protected:
		Inventory& _inventory;
		UIDocument& _document;
		Logger* _logger;

		Dictionary<InventoryIndex, UINode> _item_nodes;

		Optional<UINode> _money_text;
		Optional<UINode> _items_group;
		Optional<UINode> _item_template;
		Optional<UINode> _item_tooltip;

		void on_add(InventoryIndex index);
		void on_remove(InventoryIndex index);
		void on_change(InventoryIndex index);

		void apply_money(UInt16 value);
		void apply_item(const UINode& node, InventoryIndex index);
		void apply_item_value(const UINode& node, InventoryIndex index);
		void apply_tooltip(InventoryIndex index);

		static StringView type_to_string(ItemType type);
		static String weight_to_string(UInt16 weight);
	};
}