#include "ItemDatabase.hpp"

namespace unicore
{
	size_t ItemDatabase::get_system_memory_use() const
	{
		return sizeof(ItemDatabase) + (sizeof(Item) + sizeof(ItemId)) * _items.size();
	}

	size_t ItemDatabase::get_used_resources(Set<Shared<Resource>>& resources)
	{
		for (auto& it : _items)
			resources.insert(it.second);
		return _items.size();
	}

	ItemId ItemDatabase::add(const Item& data)
	{
		const auto index = ItemId(_last_index++);

		auto item = std::make_shared<Item>(data);
		_items.insert({ index, item });
		_event_add.invoke(index, *item);
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
		return it != _items.end() ? it->second.get() : nullptr;
	}
}