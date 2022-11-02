#include "Inventory.hpp"

namespace unicore
{
	Inventory::Inventory(const ItemDataBase& item_db, UInt16 count, UInt16 money)
		: _item_db(item_db)
		, _slots(count, ItemId_Invalid)
		, _money(money)
	{}

	void Inventory::set_money(UInt16 value)
	{
		if (_money != value)
		{
			_money = value;
			_event_money_change.invoke(_money);
		}
	}

	void Inventory::inc_money(UInt16 amount)
	{
		if (amount > 0)
		{
			_money += amount;
			_event_money_change.invoke(_money);
		}
	}

	Bool Inventory::dec_money(UInt16 amount)
	{
		if (_money >= amount)
		{
			_money -= amount;
			_event_money_change.invoke(_money);
			return true;
		}

		return false;
	}

	bool Inventory::add_item(ItemId id)
	{
		if (_item_db.has(id))
		{
			for (unsigned i = 0; i < _slots.size(); i++)
			{
				if (_slots[i] == ItemId_Invalid)
				{
					_slots[i] = id;
					_event_add_item.invoke(i, id);
					return true;
				}
			}
		}

		return false;
	}

	ItemId Inventory::get_item(unsigned index) const
	{
		return index < _slots.size() ? _slots[index] : ItemId_Invalid;
	}

	bool Inventory::has_item(ItemId id) const
	{
		for (auto slot : _slots)
		{
			if (slot == id)
				return true;
		}

		return false;
	}

	Optional<unsigned> Inventory::find_item_slot(ItemId id) const
	{
		for (unsigned i = 0; i < _slots.size(); i++)
		{
			if (_slots[i] == id)
				return i;
		}

		return std::nullopt;
	}

	bool Inventory::remove_slot(unsigned index)
	{
		if (const auto id = _slots[index]; id != ItemId_Invalid)
		{
			_slots[index] = ItemId_Invalid;
			_event_remove_item.invoke(index, id);
			return true;
		}

		return false;
	}
}