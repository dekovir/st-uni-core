#include "Inventory.hpp"

namespace unicore
{
	Inventory::Inventory(UInt16 count, UInt16 money)
		: _slots(count, nullptr)
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

	bool Inventory::add_item(const Item& item, unsigned* index)
	{
		for (unsigned i = 0; i < _slots.size(); i++)
		{
			if (_slots[i] == nullptr)
			{
				_slots[i] = &item;
				_event_add_item.invoke(i, item);
				if (index)
					*index = i;
				return true;
			}
		}

		return false;
	}

	const Item* Inventory::get_item(unsigned index) const
	{
		return _slots[index];
	}

	bool Inventory::find_item(const Item& item, unsigned* index) const
	{
		for (unsigned i = 0; i < _slots.size(); i++)
		{
			if (_slots[i] == &item)
			{
				if (index) *index = i;
				return true;
			}
		}

		return false;
	}

	bool Inventory::remove_item(unsigned index)
	{
		if (const auto item = _slots[index])
		{
			_slots[index] = nullptr;
			_event_remove_item.invoke(index, *item);
			return true;
		}

		return false;
	}
}