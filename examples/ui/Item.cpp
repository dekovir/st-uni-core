#include "Item.hpp"

namespace unicore
{
	const Dictionary<ItemType, StringView> Item::TypeNames =
	{
		{ItemType::Weapon, "Weapon"},
		{ItemType::Shield, "Shield"},
		{ItemType::Armor, "Armor"},
		{ItemType::Accessory, "Accessory"},
		{ItemType::Consumable, "Consumable"},
	};

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

	StringView Item::type_to_string(ItemType type)
	{
		const auto it = TypeNames.find(type);
		return it != TypeNames.end() ? it->second : "Error";
	}
}