#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/ui/components/ItemComponent.hpp"
#include "unicore/ui/components/InputComponent.hpp"
#include "unicore/ui/components/TextComponent.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	template<typename TKey = Int>
	class ValueGroupComponent : public GroupComponent
	{
		UC_OBJECT_EVENT(changed, TKey);
	public:
		explicit ValueGroupComponent(UIGroupType type,
			const Shared<DictionaryDataModel<TKey, String32>>& model, TKey value)
			: GroupComponent(type)
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
			on_set_value();

			_event_changed.invoke(_value);
		}

		UC_NODISCARD TKey get_value() const { return _value; }

	protected:
		Shared<DictionaryDataModel<TKey, String32>> _model;
		TKey _value;

		virtual void on_set_value() = 0;

		void update_text()
		{
			String32 text;
			if (_model->try_get_value(_value, text))
				set_attribute(UIAttribute::Value, text);
		}
	};

	template<typename TKey = Int>
	class ComboBoxComponent : public ValueGroupComponent<TKey>
	{
		UC_OBJECT_EVENT(changed, TKey);
	public:
		explicit ComboBoxComponent(const Shared<DictionaryDataModel<TKey, String32>>& model, TKey value)
			: ValueGroupComponent<TKey>(UIGroupType::Combo, model, value)
		{
		}

	protected:
		Dictionary<TKey, Shared<ItemComponent>> _nodes;

		void on_mount() override
		{
			for (const auto& key : ValueGroupComponent<TKey>::_model->keys())
			{
				String32 text;
				if (ValueGroupComponent<TKey>::_model->try_get_value(key, text))
				{
					auto item = ValueGroupComponent<TKey>::add(ItemComponent(text));
					_nodes[key] = item;

					item->set_value(key == ValueGroupComponent<TKey>::get_value());
					item->set_click_action([this, key] { ValueGroupComponent<TKey>::set_value(key); });
				}
			}
		}

		void on_dismount() override
		{
			_nodes.clear();
		}

		void on_set_value() override
		{
			for (const auto& it : _nodes)
				it.second->set_value(it.first == ValueGroupComponent<TKey>::get_value());
			ValueGroupComponent<TKey>::update_text();
		}
	};

	template<typename TKey = Int>
	using combo_box = ComboBoxComponent<TKey>;

	template<typename TKey = Int>
	class RadioGroupComponent : public ValueGroupComponent<TKey>
	{
	public:
		explicit RadioGroupComponent(const Shared<DictionaryDataModel<TKey, String32>>& model, Int value)
			: ValueGroupComponent<TKey>(UIGroupType::Vertical, model, value)
		{
		}

	protected:
		Dictionary<TKey, Shared<RadioButtonComponent>> _radio_dict;

		void on_mount() override
		{
			for (const auto& key : ValueGroupComponent<TKey>::_model->keys())
			{
				String32 value;
				if (ValueGroupComponent<TKey>::_model->try_get_value(key, value))
				{
					Shared<radio_button> radio_ref;

					ValueGroupComponent<TKey>::add(hlayout(
						ref(radio_button(key == ValueGroupComponent<TKey>::get_value()), radio_ref),
						text(value)
					));

					if (radio_ref)
					{
						_radio_dict[key] = radio_ref;
						radio_ref->set_change_callback([&, key] { ValueGroupComponent<TKey>::set_value(key); });
					}
				}
			}
		}

		void on_dismount() override
		{
			_radio_dict.clear();
		}

		void on_set_value() override
		{
			for (const auto& it : _radio_dict)
				ValueGroupComponent<TKey>::document()->set_node_attribute(
					it.second->node(), UIAttribute::Value, it.first == ValueGroupComponent<TKey>::get_value());
		}
	};

	template<typename TKey = Int>
	using radio_group = RadioGroupComponent<TKey>;
}