#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	UNICORE_MAKE_INDEX_WITH_INVALID(ItemId, UInt16);

	enum class ItemType
	{
		Weapon,
		Shield,
		Armor,
		Accessory,
		Consumable,
	};

	struct Item
	{
		ItemType type = ItemType::Consumable;
		String32 title;
		Shared<Sprite> sprite;
		UInt16 price = 0;
		UInt16 weight = 0;

		Rangei damage = RangeConsti::Zero;
		UInt16 armor = 0;

		UC_NODISCARD constexpr bool is_wearable() const
		{
			return
				type == ItemType::Weapon ||
				type == ItemType::Shield ||
				type == ItemType::Armor ||
				type == ItemType::Accessory;
		}

		UC_NODISCARD constexpr bool is_stackable() const { return type == ItemType::Consumable; }
		UC_NODISCARD constexpr bool has_status() const
		{
			return
				type == ItemType::Weapon ||
				type == ItemType::Shield ||
				type == ItemType::Armor;
		}

		static Item make_weapon(StringView32 title, UInt16 price, UInt16 weight, Rangei damage, const Shared<Sprite>& icon)
		{
			Item item;
			item.type = ItemType::Weapon;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			item.damage = damage;
			return item;
		}

		static Item make_shield(StringView32 title, UInt16 price, UInt16 weight, UInt16 armor, const Shared<Sprite>& icon)
		{
			Item item;
			item.type = ItemType::Shield;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			item.armor = armor;
			return item;
		}

		static Item make_armor(StringView32 title, UInt16 price, UInt16 weight, UInt16 armor, const Shared<Sprite>& icon)
		{
			Item item;
			item.type = ItemType::Armor;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			item.armor = armor;
			return item;
		}

		static Item make_accessory(StringView32 title, UInt16 price, UInt16 weight, const Shared<Sprite>& icon)
		{
			Item item;
			item.type = ItemType::Accessory;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			return item;
		}

		static Item make_consumable(StringView32 title, UInt16 price, UInt16 weight, const Shared<Sprite>& icon)
		{
			Item item;
			item.type = ItemType::Consumable;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			return item;
		}
	};

	using ItemValue = UInt8;

	
}