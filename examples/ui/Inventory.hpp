#pragma once
#include "Item.hpp"
#include "unicore/system/Event.hpp"

namespace unicore
{
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