#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	class InputComponent : public Component
	{
	public:
		explicit InputComponent(UIInputType type);
	};

	template<typename T>
	class TypedInputComponent : public InputComponent
	{
	public:
		explicit TypedInputComponent(UIInputType type) : InputComponent(type) {}
		TypedInputComponent(UIInputType type, T value) : InputComponent(type) { set_value(value); }

		void set_value(T value) { set_attribute(UIAttribute::Value, value); }
		UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }
	};

	class TextInputComponent : public InputComponent
	{
	public:
		TextInputComponent();
		explicit TextInputComponent(StringView32 text);

		void set_value(StringView32 value);
		UC_NODISCARD String32 get_value() const;
	};

	using input_text = TextInputComponent;

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
	};

	template<typename T>
	using input_int_t = IntegerInputComponent<T>;

	using input_int = IntegerInputComponent<Int>;
	using input_int64 = IntegerInputComponent<Int64>;

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
	};

	using input_float = FloatInputComponent<Float>;
	using input_double = FloatInputComponent<Double>;

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