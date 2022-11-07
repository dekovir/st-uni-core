#include "ItemDatabase.hpp"

namespace unicore
{
	ItemId ItemDatabase::add(const Item& item)
	{
		const auto index = ItemId(_last_index++);
		_items.insert({ index, item });
		return index;
	}

	bool ItemDatabase::has(ItemId id) const
	{
		if (id != ItemId_Invalid)
		{
			const auto it = _items.find(id);
			if (it != _items.end())
				return true;
		}

		return false;
	}

	const Item* ItemDatabase::get(ItemId id) const
	{
		const auto it = _items.find(id);
		return it != _items.end() ? &it->second : nullptr;
	}
}