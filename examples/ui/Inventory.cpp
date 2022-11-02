#include "Inventory.hpp"

namespace unicore
{
	Inventory::Inventory(const ItemDataBase& item_db)
		: _item_db(item_db)
	{}

	InventoryIndex Inventory::add_item(ItemId id, ItemValue value)
	{
		if (const auto item = _item_db.get(id))
		{
			if (item->is_stackable())
			{
				for (auto& [index, info] : _items)
				{
					if (info.id == id)
					{
						info.value += value;
						_event_item_value_changed.invoke(index);
						return index;
					}
				}
			}

			const InventoryIndex index(_last_index++);
			_items[index] = { id, value };
			_event_add_item.invoke(index);
			return index;
		}

		return InventoryIndex_Invalid;
	}

	ItemId Inventory::get_index_id(InventoryIndex index) const
	{
		const auto it = _items.find(index);
		return it != _items.end() ? it->second.id : ItemId_Invalid;
	}

	ItemValue Inventory::get_index_value(InventoryIndex index) const
	{
		const auto it = _items.find(index);
		return it != _items.end() ? it->second.value : 0;
	}

	bool Inventory::has_item(ItemId id) const
	{
		return find_item_index(id) != InventoryIndex_Invalid;
	}

	InventoryIndex Inventory::find_item_index(ItemId id) const
	{
		for (const auto& it : _items)
		{
			if (it.second.id == id)
				return it.first;
		}

		return InventoryIndex_Invalid;
	}

	bool Inventory::remove_index(InventoryIndex index)
	{
		if (const auto it = _items.find(index); it != _items.end())
		{
			_items.erase(index);
			_event_remove_item.invoke(index);
			return true;
		}

		return false;
	}
}