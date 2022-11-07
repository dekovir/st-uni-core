#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/resource/Resource.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	enum class ItemType : uint8_t
	{
		Weapon,
		Shield,
		Armor,
		Accessory,
		Consumable,
	};

	class Item : public Resource
	{
		UC_OBJECT(Item, Resource)
	public:

		UC_NODISCARD size_t get_system_memory_use() const override;
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		ItemType item_type = ItemType::Consumable;
		String32 title;
		Shared<Sprite> sprite;
		UInt16 price = 0;
		UInt16 weight = 0;

		Rangei damage = RangeConsti::Zero;
		UInt16 armor = 0;

		UC_NODISCARD bool is_wearable() const;
		UC_NODISCARD bool is_stackable() const;
		UC_NODISCARD bool has_status() const;

		static Item make_weapon(StringView32 title, UInt16 price, UInt16 weight, Rangei damage, const Shared<Sprite>& icon)
		{
			Item item;
			item.item_type = ItemType::Weapon;
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
			item.item_type = ItemType::Shield;
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
			item.item_type = ItemType::Armor;
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
			item.item_type = ItemType::Accessory;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			return item;
		}

		static Item make_consumable(StringView32 title, UInt16 price, UInt16 weight, const Shared<Sprite>& icon)
		{
			Item item;
			item.item_type = ItemType::Consumable;
			item.title = title;
			item.price = price;
			item.weight = weight;
			item.sprite = icon;
			return item;
		}
	};

	using ItemValue = UInt8;
}