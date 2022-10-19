#include "example.hpp"
#include "unicore/ui/UIPlugin.hpp"
#include "unicore/renderer/Sprite.hpp"
#include "unicore/resource/ResourceCache.hpp"

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
		explicit Inventory(UInt16 count)
			: _slots(count, nullptr)
		{}

		UC_NODISCARD auto size() const { return _slots.size(); }

		bool add_item(const Item& item, unsigned* index = nullptr)
		{
			for (unsigned i = 0; i < _slots.size(); i++)
			{
				if (_slots[i] == nullptr)
				{
					_slots[i] = &item;
					_event_add_item.invoke(i, item);
					if (index)
						*index = i;
					return true;
				}
			}

			return false;
		}

		UC_NODISCARD const Item* get_item(unsigned index) const
		{
			return _slots[index];
		}

		UC_NODISCARD bool find_item(const Item& item, unsigned* index = nullptr) const
		{
			for (unsigned i = 0; i < _slots.size(); i++)
			{
				if (_slots[i] == &item)
				{
					if (index) *index = i;
					return true;
				}
			}

			return false;
		}

		UC_NODISCARD bool remove_item(unsigned index)
		{
			if (const auto item = _slots[index])
			{
				_slots[index] = nullptr;
				_event_remove_item.invoke(index, *item);
				return true;
			}

			return false;
		}

	protected:
		List<const Item*> _slots;
	};

	class InventoryBackend : public UIBackend
	{
	public:
		InventoryBackend()
			: _bind_add_item([this](auto index, auto& item) { on_add_item(index, item); })
			, _bind_remove_item([this](auto index, auto& item) { on_remove_item(index, item); })
		{
		}

		UC_NODISCARD const Shared<Inventory>& model() const { return _inventory; }

		void set_model(const Shared<Inventory>& inventory)
		{
			if (_inventory == inventory)
				return;

			if (_inventory)
			{
				_inventory->on_add_item() -= _bind_add_item;
				_inventory->on_remove_item() -= _bind_remove_item;
			}

			_inventory = inventory;

			if (_inventory)
			{
				_inventory->on_add_item() += _bind_add_item;
				_inventory->on_remove_item() += _bind_remove_item;
			}

			on_rebuild();
		}

		void render(UIElementList& elements)
		{
			UIElementList items_elements;
			if (_inventory)
			{
				const auto count = _inventory->size();
				for (unsigned i = 0; i < count; i++)
				{
					const auto item = _inventory->get_item(i);
					render_item(i, item, items_elements);
				}
			}

			elements.push_back(
				{
					1,
					UIElementData::make_div(),
					{
						items_elements
					}
				}
			);
		}

	protected:
		const Action<unsigned, const Item&> _bind_add_item;
		const Action<unsigned, const Item&> _bind_remove_item;

		Shared<Inventory> _inventory;

		void on_rebuild()
		{
			UIElementList elements;
			render(elements);
			_event_rebuild.invoke(elements);
		}

		void on_add_item(unsigned index, const Item& item)
		{}

		void on_remove_item(unsigned index, const Item& item)
		{}

		void render_item(unsigned index, const Item* item, UIElementList& elements)
		{
		}
	};

	class ExampleUI : public Example
	{
	public:
		explicit ExampleUI(const ExampleContext& context)
			: Example(context)
		{
		}

		void load(IResourceCache& resources) override
		{
		}

		void update() override
		{
		}

		void draw() const override
		{
		}

	protected:
		Shared<Inventory> _inventory;
	};

	UC_EXAMPLE_REGISTER(ExampleUI, "UI");
}