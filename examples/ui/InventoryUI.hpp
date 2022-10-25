#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class InventoryUI
	{
	public:
		InventoryUI(Inventory& inventory, UIDocument& document);

	protected:
		Inventory& _inventory;
		UIDocument& _document;

		void on_add_item(unsigned index, const Item& item);
		void on_remove_item(unsigned index, const Item& item);
	};
}