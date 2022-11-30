#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/resource/Resource.hpp"
#include "unicore/system/Event.hpp"
#include "Item.hpp"

namespace unicore
{
	UNICORE_MAKE_INDEX_WITH_INVALID(ItemId, UInt16);

	class ItemDatabase : public Resource
	{
		UC_OBJECT_EVENT(add, ItemId, const Item&);
		//UC_OBJECT_EVENT(remove, ItemId);
	public:
		UC_NODISCARD size_t get_system_memory_use() const override;
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		UC_NODISCARD Size size() const { return _items.size(); }

		ItemId add(const Item& item);

		UC_NODISCARD bool has(ItemId id) const;

		UC_NODISCARD const Item* get(ItemId id) const;

		UC_NODISCARD const auto& items() const { return _items; }
		UC_NODISCARD auto begin() const { return _items.begin(); }
		UC_NODISCARD auto end() const { return _items.end(); }

	protected:
		ItemId::TypeValue _last_index = 0;
		Dictionary<ItemId, Shared<Item>> _items;
	};
}