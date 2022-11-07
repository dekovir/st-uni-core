#include "Item.hpp"

namespace unicore
{
	size_t Item::get_system_memory_use() const
	{
		return sizeof(Item);
	}

	size_t Item::get_used_resources(Set<Shared<Resource>>& resources)
	{
		if (sprite)
		{
			resources.insert(sprite);
			return 1;
		}

		return 0;
	}

	bool Item::is_wearable() const
	{
		return
			item_type == ItemType::Weapon ||
			item_type == ItemType::Shield ||
			item_type == ItemType::Armor ||
			item_type == ItemType::Accessory;
	}

	bool Item::is_stackable() const
	{
		return item_type == ItemType::Consumable;
	}

	bool Item::has_status() const
	{
		return
			item_type == ItemType::Weapon ||
			item_type == ItemType::Shield ||
			item_type == ItemType::Armor;
	}
}