#pragma once
#include "Item.hpp"

namespace unicore
{
	class ItemDatabase
	{
	public:
		UC_NODISCARD Size size() const { return _items.size(); }

		ItemId add(const Item& item);

		UC_NODISCARD bool has(ItemId id) const;

		UC_NODISCARD const Item* get(ItemId id) const;

		UC_NODISCARD auto begin() const { return _items.begin(); }
		UC_NODISCARD auto end() const { return _items.end(); }

	protected:
		ItemId::TypeValue _last_index = 0;
		Dictionary<ItemId, Item> _items;
	};
}