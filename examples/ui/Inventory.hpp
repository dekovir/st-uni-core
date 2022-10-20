#pragma once
#include "unicore/system/Event.hpp"
#include "unicore/renderer/Sprite.hpp"

namespace unicore
{
	struct Item
	{
		String32 title;
		Int16 price;
		Shared<Sprite> sprite;
	};

	class Inventory // Model
	{
		UC_OBJECT_EVENT(add_item, unsigned, const Item&);
		UC_OBJECT_EVENT(remove_item, unsigned, const Item&);
	public:
		explicit Inventory(UInt16 count);

		UC_NODISCARD auto size() const { return _slots.size(); }

		bool add_item(const Item& item, unsigned* index = nullptr);

		UC_NODISCARD const Item* get_item(unsigned index) const;

		UC_NODISCARD bool find_item(const Item& item, unsigned* index = nullptr) const;

		UC_NODISCARD bool remove_item(unsigned index);

	protected:
		List<const Item*> _slots;
	};
}