#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	struct ItemId_Tag {};
	using ItemId = Index<UInt16, ItemId_Tag>;
	static constexpr ItemId ItemId_Invalid = ItemId(std::numeric_limits<ItemId::TypeValue>::max());

	enum class ItemType
	{
		Weapon,
		Armor,
		Accessory,
		Consumable,
	};

	struct Item
	{
		String32 title;
		ItemType type;
		Int16 price;
		Shared<Sprite> sprite;
	};

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

	class Inventory
	{
		UC_OBJECT_EVENT(money_change, UInt16);
		UC_OBJECT_EVENT(add_item, unsigned, ItemId);
		UC_OBJECT_EVENT(remove_item, unsigned, ItemId);
	public:
		explicit Inventory(const ItemDataBase& item_db, UInt16 count, UInt16 money = 0);

		UC_NODISCARD const ItemDataBase& database() const { return _item_db; }

		UC_NODISCARD UInt16 money() const { return _money; }

		void set_money(UInt16 value);
		void inc_money(UInt16 amount);
		Bool dec_money(UInt16 amount);

		UC_NODISCARD auto item_count() const { return _slots.size(); }

		bool add_item(ItemId id);

		UC_NODISCARD ItemId get_item(unsigned index) const;

		UC_NODISCARD bool has_item(ItemId id) const;
		UC_NODISCARD Optional<unsigned> find_item_slot(ItemId id) const;

		UC_NODISCARD bool remove_slot(unsigned index);

	protected:
		const ItemDataBase& _item_db;
		List<ItemId> _slots;
		UInt16 _money;
	};
}