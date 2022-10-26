#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	struct ItemId_Tag {};
	using ItemId = Index<UInt16, ItemId_Tag>;

	enum class ItemType
	{
		Weapon,
		Armor,
		Accessory,
		Consumable,
	};

	struct Item
	{
		//ItemId id;
		String32 title;
		ItemType type;
		Int16 price;
		Shared<Sprite> sprite;
	};

	class Inventory
	{
		UC_OBJECT_EVENT(money_change, unsigned);
		UC_OBJECT_EVENT(add_item, unsigned, const Item&);
		UC_OBJECT_EVENT(remove_item, unsigned, const Item&);
	public:
		explicit Inventory(UInt16 count, UInt16 money = 0);

		UC_NODISCARD UInt16 money() const { return _money; }

		void set_money(UInt16 value);
		void inc_money(UInt16 amount);
		Bool dec_money(UInt16 amount);

		UC_NODISCARD auto size() const { return _slots.size(); }

		bool add_item(const Item& item, unsigned* index = nullptr);

		UC_NODISCARD const Item* get_item(unsigned index) const;

		UC_NODISCARD bool find_item(const Item& item, unsigned* index = nullptr) const;

		UC_NODISCARD bool remove_item(unsigned index);

	protected:
		List<const Item*> _slots;
		UInt16 _money;
	};
}