#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/ui/components/ItemComponent.hpp"
#include "unicore/ui/UIDataModel.hpp"

namespace unicore::ui
{
	template<typename TKey = Int>
	class ComboBoxComponent : public GroupComponent
	{
		UC_OBJECT_EVENT(changed, TKey);
	public:
		explicit ComboBoxComponent(const Shared<DictionaryDataModel<TKey, String32>>& model, TKey value)
			: GroupComponent(UIGroupType::Combo)
			, _model(model)
			, _value(value)
		{
			update_text();
		}

		void set_value(TKey value)
		{
			if (_value == value)
				return;

			_value = value;
			for (const auto& it : _nodes)
				it.second->set_value(it.first == _value);
			update_text();

			_event_changed.invoke(_value);
		}

		UC_NODISCARD TKey get_value() const { return _value; }

	protected:
		Shared<DictionaryDataModel<TKey, String32>> _model;
		Dictionary<TKey, Shared<ItemComponent>> _nodes;
		TKey _value;

		void on_mount() override
		{
			for (const auto& key : _model->keys())
			{
				String32 text;
				if (_model->try_get(key, text))
				{
					auto item = add(ItemComponent(text));
					_nodes[key] = item;

					item->set_value(key == _value);
					item->set_click_action([this, key] { set_value(key); });
				}
			}
		}

		void on_dismount() override
		{
			_nodes.clear();
		}

		void update_text()
		{
			String32 text;
			if (_model->try_get(_value, text))
				set_attribute(UIAttribute::Value, text);
		}
	};

	template<typename TKey = Int>
	using combo_box = ComboBoxComponent<TKey>;
}