#include "InventoryUI.hpp"

namespace unicore
{
	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document)
		: _inventory(inventory)
		, _document(document)
	{
		_inventory.on_add_item() += [&](auto index, auto& item) { on_add_item(index, item); };
		_inventory.on_remove_item() += [&](auto index, auto& item) { on_remove_item(index, item); };
	}

	void InventoryUI::on_add_item(unsigned index, const Item& item)
	{
	}

	void InventoryUI::on_remove_item(unsigned index, const Item& item)
	{
	}
}