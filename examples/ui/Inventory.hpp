#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/system/Event.hpp"
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

	using ItemValue = UInt16;

	class ItemDatabase
	{
	public:
		UC_NODISCARD Size size() const { return _items.size(); }

		ItemId add(const Item& item)
		{
			const auto index = ItemId(_last_index++);
			_items.insert({ index, item });
			return index;
		}

		UC_NODISCARD bool has(ItemId id) const
		{
			if (id != ItemId_Invalid)
			{
				const auto it = _items.find(id);
				if (it != _items.end())
					return true;
			}

			return false;
		}

		UC_NODISCARD const Item* get(ItemId id) const
		{
			const auto it = _items.find(id);
			return it != _items.end() ? &it->second : nullptr;
		}

		UC_NODISCARD auto begin() const { return _items.begin(); }
		UC_NODISCARD auto end() const { return _items.end(); }

	protected:
		ItemId::TypeValue _last_index = 0;
		Dictionary<ItemId, Item> _items;
	};

	UNICORE_MAKE_INDEX_WITH_INVALID(InventoryIndex, UInt16);

	class Inventory
	{
		UC_OBJECT_EVENT(add, InventoryIndex);
		UC_OBJECT_EVENT(remove, InventoryIndex);
		UC_OBJECT_EVENT(changed, InventoryIndex);
	public:
		explicit Inventory(const ItemDatabase& database);

		UC_NODISCARD const ItemDatabase& database() const { return _database; }

		UC_NODISCARD auto items_count() const { return _items.size(); }

		InventoryIndex add_item(ItemId id, ItemValue value = 1);

		UC_NODISCARD ItemId get_index_id(InventoryIndex index) const;
		UC_NODISCARD ItemValue get_index_value(InventoryIndex index) const;

		UC_NODISCARD bool has_item(ItemId id) const;
		UC_NODISCARD InventoryIndex find_item_index(ItemId id) const;

		UC_NODISCARD bool remove_index(InventoryIndex index);

	protected:
		struct ItemInfo
		{
			ItemId id;
			ItemValue value;
		};

		const ItemDatabase& _database;
		Dictionary<InventoryIndex, ItemInfo> _items;
		InventoryIndex::TypeValue _last_index = 0;
	};
}