#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class InventoryUI
	{
	public:
		InventoryUI(Inventory& inventory, UIDocument& document,
			const UINode& parent = UINode::Empty, Logger* logger = nullptr);
		InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger = nullptr);

	protected:
		Inventory& _inventory;
		UIDocument& _document;
		Logger* _logger;

		Dictionary<InventoryIndex, UINode> _item_nodes;

		UINode _money_text;
		UINode _items_group;
		UINode _item_template;
		UINode _item_tooltip;

		void on_add(InventoryIndex index);
		void on_remove(InventoryIndex index);
		void on_change(InventoryIndex index);

		void apply_money(UInt16 value);
		void apply_item(const UINode& node, InventoryIndex index);
		void apply_item_value(const UINode& node, InventoryIndex index);
		void apply_tooltip(InventoryIndex index);

		static String weight_to_string(UInt16 weight);
	};
}