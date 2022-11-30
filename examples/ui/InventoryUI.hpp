#pragma once
#include "unicore/remoteui/Document.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class InventoryUI
	{
	public:
		InventoryUI(Inventory& inventory, remoteui::Document& document,
			const remoteui::Element& parent = remoteui::Element::Empty, Logger* logger = nullptr);
		InventoryUI(Inventory& inventory, remoteui::Document& document,
			Logger* logger = nullptr);

	protected:
		Inventory& _inventory;
		remoteui::Document& _document;
		Logger* _logger;

		Dictionary<InventoryIndex, remoteui::Element> _item_nodes;

		remoteui::Element _money_text;
		remoteui::Element _items_group;
		remoteui::Element _item_template;
		remoteui::Element _item_tooltip;

		void on_add(InventoryIndex index);
		void on_remove(InventoryIndex index);
		void on_change(InventoryIndex index);

		void apply_money(UInt16 value);
		void apply_item(const remoteui::Element& node, InventoryIndex index);
		void apply_item_value(const remoteui::Element& node, InventoryIndex index);
		void apply_tooltip(InventoryIndex index);

		static String weight_to_string(UInt16 weight);
	};
}