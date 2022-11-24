#pragma once
#include "unicore/ui/elements/TypedElement.hpp"

namespace unicore::ui
{
	// ValueInputElement /////////////////////////////////////////////////////////
	template<typename TTemplate, typename TValue>
	class ValueInputElement : public TypedElement<TTemplate>
	{
		UC_OBJECT_EVENT(changed, sfinae::ConstRefType<TValue>);
	public:
		using ValueType = TValue;
		using ValueArg = sfinae::ConstRefType<ValueType>;

		explicit ValueInputElement(TValue value = {})
			: _value(value)
		{}

		void set_value(ValueArg value)
		{
			if (_value == value) return;

			_value = value;
			TypedElement<TTemplate>::rebuild();
		}

	protected:
		TValue _value;

		void apply_params(TTemplate& item) override
		{
			TypedElement<TTemplate>::apply_params(item);

			item.set_params({ attr::Value(_value) });

			//if (!_event_changed.empty())
			//{
			//	item.set_params({ action::OnChange([&](const UINode& node)
			//	{
			//		_value = node.value().get<TValue>();
			//		_event_changed(_value);
			//	}) });
			//}
		}
	};

	using input_text = ValueInputElement<InputText, String32>;
	using toggle = ValueInputElement<InputToggle, Bool>;
	using radio_button = ValueInputElement<InputRadio, Bool>;

	class ItemElement : public ValueInputElement<InputItem, Bool>
	{
	public:
		ItemElement() = default;

		explicit ItemElement(StringView32 text)
			: _text(text)
		{}

		void set_text(StringView32 text)
		{
			if (_text == text) return;

			_text = text;
			rebuild();
		}

	protected:
		String32 _text;

		void apply_params(InputItem& item) override
		{
			if (!_text.empty())
				item.set_params({ attr::Text(_text) });
		}
	};

	using item = ItemElement;

	// NumberInputElement ////////////////////////////////////////////////////////
	template<typename TTemplate, typename TValue,
		std::enable_if_t<sfinae::is_numeric_v<TValue>>* = nullptr>
	class NumberInputElement : public ValueInputElement<TTemplate, TValue>
	{
	public:
		explicit NumberInputElement(TValue value = {})
			: ValueInputElement<TTemplate, TValue>(value)
		{}

		explicit NumberInputElement(TValue value,
			const typename ValueInputElement<TTemplate, TValue>::Event_changed::ActionType& action)
			: ValueInputElement<TTemplate, TValue>(value)
		{
			ValueInputElement<TTemplate, TValue>::_event_changed.add(action);
		}

		explicit NumberInputElement(TValue value, TValue step)
			: NumberInputElement(value)
		{
			_step = step;
		}

		explicit NumberInputElement(const Range<TValue>& range, TValue value)
			: NumberInputElement(value)
		{
			_range = range;
		}

		explicit NumberInputElement(const Range<TValue>& range, TValue value, TValue step)
			: NumberInputElement(value)
		{
			_range = range;
			_step = step;
		}

	protected:
		Optional<Range<TValue>> _range;
		Optional<TValue> _step;

		void apply_params(TTemplate& item) override
		{
			ValueInputElement<TTemplate, TValue>::apply_params(item);

			if (_step.has_value())
				item.set_params({ attr::Step(_step.value()) });

			if (_range.has_value())
				item.set_params({ attr::Min(_range->min), attr::Max(_range->max) });
		}
	};

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	using input_int_t = NumberInputElement<InputInteger, T>;
	using input_int = input_int_t<Int>;

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	using input_float_t = NumberInputElement<InputFloat, T>;
	using input_float = input_float_t<Float>;

	// NumberInputElement ////////////////////////////////////////////////////////
	template<typename TTemplate, typename TValue,
		std::enable_if_t<sfinae::is_numeric_v<TValue>>* = nullptr>
	class RangeInputElement : public ValueInputElement<TTemplate, TValue>
	{
	public:
		explicit RangeInputElement(TValue value = {})
			: ValueInputElement<TTemplate, TValue>(value)
		{}

		explicit RangeInputElement(const Range<TValue>& range, TValue value)
			: RangeInputElement(value)
		{
			_range = range;
		}

	protected:
		Optional<Range<TValue>> _range;

		void apply_params(TTemplate& item) override
		{
			ValueInputElement<TTemplate, TValue>::apply_params(item);

			if (_range.has_value())
				item.set_params({ attr::Min(_range->min), attr::Max(_range->max) });
		}
	};

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	using range_int_t = RangeInputElement<InputSliderI, T>;
	using range_int = range_int_t<Int>;

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	using range_float_t = RangeInputElement<InputSliderF, T>;
	using range_float = range_float_t<Float>;

	// ButtonElement /////////////////////////////////////////////////////////////
	class ButtonElement : public TypedElement<InputButton>
	{
		UC_OBJECT_EVENT(clicked, Bool);
	public:
		ButtonElement() = default;

		explicit ButtonElement(StringView32 text)
			: _text(text)
		{}

		ButtonElement(StringView32 text,
			const Event_clicked::ActionType& action)
			: ButtonElement(text)
		{
			_event_clicked.add(action);
		}

		void set_text(StringView32 text)
		{
			if (_text == text) return;

			_text = text;
			rebuild();
		}

	protected:
		String32 _text;

		void apply_params(InputButton& temp) override
		{
			TypedElement<InputButton>::apply_params(temp);

			if (!_text.empty())
				temp.set_params({ attr::Text(_text) });

			if (!_event_clicked.empty())
				temp.set_params({ action::OnClick([&] { _event_clicked.invoke(true); }) });
		}
	};

	using button = ButtonElement;
}