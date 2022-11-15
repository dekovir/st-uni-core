#include "unicore/ui/components/InputComponent.hpp"

namespace unicore::ui
{
	InputComponent::InputComponent(UIInputType type)
		: Component(UINodeTag::Input)
	{
		set_attribute(UIAttribute::Type, type);
	}

	// TextInputComponent ////////////////////////////////////////////////////////
	TextInputComponent::TextInputComponent()
		: InputComponent(UIInputType::Text)
	{}

	TextInputComponent::TextInputComponent(StringView32 text)
		: InputComponent(UIInputType::Text)
	{
		set_value(text);
	}

	void TextInputComponent::set_value(StringView32 value)
	{
		set_attribute(UIAttribute::Value, value);
	}

	String32 TextInputComponent::get_value() const
	{
		return get_attribute(UIAttribute::Value).get_string32();
	}

	// ToggleComponent ///////////////////////////////////////////////////////////
	ToggleComponent::ToggleComponent(Bool value)
		: TypedInputComponent(UIInputType::Toggle, value)
	{ }

	// RadioButtonComponent //////////////////////////////////////////////////////
	RadioButtonComponent::RadioButtonComponent(Bool value)
		: TypedInputComponent(UIInputType::Radio, value)
	{ }

	// ButtonComponent ///////////////////////////////////////////////////////////
	ButtonComponent::ButtonComponent() : InputComponent(UIInputType::Button)
	{}

	ButtonComponent::ButtonComponent(StringView32 text) : InputComponent(UIInputType::Button)
	{
		set_text(text);
	}

	void ButtonComponent::set_text(StringView32 text)
	{
		set_attribute(UIAttribute::Text, text);
	}

	String32 ButtonComponent::text() const
	{
		return get_attribute(UIAttribute::Text).get_string32();
	}

	void ButtonComponent::set_click_action(const UIAction& action)
	{
		set_action(UIActionType::OnClick, action);
	}
}