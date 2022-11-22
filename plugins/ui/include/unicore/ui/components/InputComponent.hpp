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
		UC_OBJECT_EVENT(change, T);
	public:
		using CallbackType = typename Event_change::ActionType;

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
			on_change().add(callback);
		}

		void set_value(T value) { set_attribute(UIAttribute::Value, value); }
		UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }

	protected:
		void apply_options(UINodeOptions& options) override
		{
			InputComponent::apply_options(options);

			options.actions[UIActionType::OnChange] =
				[&] { _event_change(get_value()); };
		}
	};

	class TextInputComponent : public InputComponent
	{
		UC_OBJECT_EVENT(change, StringView32);
	public:
		TextInputComponent()
			: InputComponent(UIInputType::Text)
		{}

		explicit TextInputComponent(StringView32 text)
			: InputComponent(UIInputType::Text)
		{
			set_value(text);
		}

		explicit TextInputComponent(StringView32 text, const Event_change::ActionType& callback)
			: TextInputComponent(text)
		{
			on_change().add(callback);
		}

		void set_value(StringView32 value)
		{
			set_attribute(UIAttribute::Value, value);
		}

		UC_NODISCARD String32 get_value() const
		{
			return get_attribute(UIAttribute::Value).get_string32();
		}

	protected:
		void apply_options(UINodeOptions& options) override
		{
			InputComponent::apply_options(options);

			options.actions[UIActionType::OnChange] =
				[&] { _event_change(get_value()); };
		}
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
		UC_OBJECT_EVENT(clicked, Bool);
	public:
		ButtonComponent();
		explicit ButtonComponent(StringView32 text);
		ButtonComponent(StringView32 text, const Event_clicked::ActionType& action);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;

	protected:
		void apply_options(UINodeOptions& options) override;
	};

	using button = ButtonComponent;

	template<typename T,
		std::enable_if_t<sfinae::is_numeric_v<T>>* = nullptr>
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

		explicit FloatInputComponent(T value,
			const typename NumberInputComponent<T>::CallbackType& callback)
			: NumberInputComponent<T>(value, callback)
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
		std::enable_if_t<sfinae::is_numeric_v<T>>* = nullptr>
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