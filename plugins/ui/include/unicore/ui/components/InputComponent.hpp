#pragma once
#include "unicore/ui/components/NodeComponent.hpp"

namespace unicore::ui
{
	class InputComponent : public NodeComponent
	{
	public:
		explicit InputComponent(UIInputType type);
	};

	template<typename T>
	class TypedInputComponent : public InputComponent
	{
	public:
		using CallbackType = std::function<void(T)>;

		explicit TypedInputComponent(UIInputType type)
			: InputComponent(type) {}

		TypedInputComponent(UIInputType type, T value)
			: TypedInputComponent(type)
		{
			set_value(value);
		}

		TypedInputComponent(UIInputType type, T value, const CallbackType& callback)
			: TypedInputComponent(type, value)
		{
			set_change_callback(callback);
		}

		void set_value(T value) { set_attribute(UIAttribute::Value, value); }
		UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }

		void set_change_callback(const CallbackType& callback)
		{
			set_action(UIActionType::OnChange,
				[&] { callback(get_value()); });
		}
	};

	template<typename TChar,
		std::enable_if<unicore::sfinae::is_char_v<TChar>>* = nullptr>
	class TextInputComponent : public InputComponent
	{
	public:
		using ChangeFunc = std::function<void(BasicStringView<TChar>)>;

		TextInputComponent()
			: InputComponent(UIInputType::Text)
		{}

		explicit TextInputComponent(BasicStringView<TChar> text)
			: InputComponent(UIInputType::Text)
		{
			set_value(text);
		}

		explicit TextInputComponent(BasicStringView<TChar> text, const ChangeFunc& callback)
			: InputComponent(text)
		{
			set_callback(callback);
		}

		void set_value(BasicStringView<TChar> value)
		{
			set_attribute(UIAttribute::Value, value);
		}

		UC_NODISCARD BasicString<TChar> get_value() const
		{
			return Unicode::to<TChar>(get_attribute(UIAttribute::Value).get_string32());
		}

		void set_callback(const ChangeFunc& callback)
		{
			set_action(UIActionType::OnChange,
				[this, callback] { callback(get_value()); });
		}
	};

	template<typename TChar>
	using input_text_t = TextInputComponent<TChar>;

	using input_text8 = input_text_t<Char8>;
	using input_textW = input_text_t<CharW>;
	using input_text16 = input_text_t<Char16>;
	using input_text32 = input_text_t<Char32>;

	using input_text = input_text32;

	class ToggleComponent : public TypedInputComponent<Bool>
	{
	public:
		explicit ToggleComponent(Bool value = false);
	};

	using toggle = ToggleComponent;

	class RadioButtonComponent : public TypedInputComponent<Bool>
	{
	public:
		explicit RadioButtonComponent(Bool value = false);
	};

	using radio_button = RadioButtonComponent;

	class ButtonComponent : public InputComponent
	{
	public:
		ButtonComponent();
		explicit ButtonComponent(StringView32 text);
		ButtonComponent(StringView32 text, const UIAction& action);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;

		void set_click_action(const UIAction& action);
	};

	using button = ButtonComponent;

	template<typename T,
		std::enable_if_t<unicore::sfinae::is_numeric_v<T>>* = nullptr>
	class NumberInputComponent : public TypedInputComponent<T>
	{
	public:
		static constexpr auto InputType = std::is_floating_point_v<T>
			? UIInputType::Float : UIInputType::Integer;

		NumberInputComponent()
			: TypedInputComponent<T>(InputType)
		{}

		explicit NumberInputComponent(T value)
			: TypedInputComponent<T>(InputType)
		{
			TypedInputComponent<T>::set_value(value);
		}

		explicit NumberInputComponent(T value,
			const typename TypedInputComponent<T>::CallbackType& callback)
			: TypedInputComponent<T>(InputType, value, callback)
		{
		}

		explicit NumberInputComponent(const Range<T>& range)
			: NumberInputComponent()
		{
			TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
			TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
		}

		NumberInputComponent(const Range<T>& range, T value)
			: NumberInputComponent(value)
		{
			TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
			TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
		}

		NumberInputComponent(const Range<T>& range, T value,
			const typename TypedInputComponent<T>::CallbackType& callback)
			: NumberInputComponent(value, callback)
		{
			TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
			TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
		}

		void set_step(T value)
		{
			TypedInputComponent<T>::set_attribute(UIAttribute::Step, value);
		}

		UC_NODISCARD T get_step() const
		{
			return TypedInputComponent<T>::get_attribute(UIAttribute::Step).template get_integral<T>();
		}
	};

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	class IntegerInputComponent : public NumberInputComponent<T>
	{
	public:
		explicit IntegerInputComponent()
			: NumberInputComponent<T>({ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() })
		{
		}

		explicit IntegerInputComponent(T value)
			: NumberInputComponent<T>({ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() }, value)
		{
		}

		explicit IntegerInputComponent(T value,
			const typename NumberInputComponent<T>::CallbackType& callback)
			: NumberInputComponent<T>({ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() }, value, callback)
		{
		}
	};

	template<typename T>
	using input_int_t = IntegerInputComponent<T>;

	using input_int = input_int_t<Int>;
	using input_int64 = input_int_t<Int64>;

	template<typename T,
		std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	class FloatInputComponent : public NumberInputComponent<T>
	{
	public:
		FloatInputComponent() = default;

		explicit FloatInputComponent(T value)
			: NumberInputComponent<T>(value)
		{
		}

		explicit FloatInputComponent(T value, T step)
			: NumberInputComponent<T>(value)
		{
			NumberInputComponent<T>::set_step(step);
		}

		explicit FloatInputComponent(T value, T step,
			const typename NumberInputComponent<T>::CallbackType& callback)
			: NumberInputComponent<T>(value, callback)
		{
			NumberInputComponent<T>::set_step(step);
		}
	};

	template<typename T>
	using input_float_t = FloatInputComponent<T>;

	using input_float = input_float_t<Float>;
	using input_double = input_float_t<Double>;

	template<typename T,
		std::enable_if_t<unicore::sfinae::is_numeric_v<T>>* = nullptr>
	class RangeComponent : public TypedInputComponent<T>
	{
	public:
		static constexpr auto InputType = std::is_floating_point_v<T>
			? UIInputType::RangeF : UIInputType::RangeI;

		explicit RangeComponent(const Range<T>& range, T value)
			: TypedInputComponent<T>(InputType)
		{
			TypedInputComponent<T>::set_value(value);
			TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
			TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
		}
	};

	using range_int = RangeComponent<Int>;
	using range_float = RangeComponent<Float>;
	using range_double = RangeComponent<Double>;
}