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

		Optional<UINode> _items_group;
		Optional<UINode> _items_template;

		void on_add_item(unsigned index, const Item& item);
		void on_remove_item(unsigned index, const Item& item);
	};
}