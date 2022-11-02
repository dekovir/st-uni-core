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
		String32 title;
		ItemType type;
		UInt16 price;
		UInt16 weight;
		Shared<Sprite> sprite;

		UC_NODISCARD constexpr bool is_wearable() const
		{
			return
				type == ItemType::Weapon ||
				type == ItemType::Shield ||
				type == ItemType::Armor ||
				type == ItemType::Accessory;
		}

		UC_NODISCARD constexpr bool is_stackable() const { return type == ItemType::Consumable; }
	};

	using ItemValue = UInt16;

	class ItemDataBase
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
		UC_OBJECT_EVENT(add_item, InventoryIndex);
		UC_OBJECT_EVENT(remove_item, InventoryIndex);
		UC_OBJECT_EVENT(item_value_changed, InventoryIndex);
	public:
		explicit Inventory(const ItemDataBase& item_db);

		UC_NODISCARD const ItemDataBase& database() const { return _item_db; }

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

		const ItemDataBase& _item_db;
		Dictionary<InventoryIndex, ItemInfo> _items;
		InventoryIndex::TypeValue _last_index = 0;
	};
}