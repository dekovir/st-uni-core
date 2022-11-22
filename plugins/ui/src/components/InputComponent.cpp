#include "unicore/ui/components/InputComponent.hpp"

namespace unicore::ui
{
	InputComponent::InputComponent(UIInputType type)
		: NodeComponent(UINodeTag::Input)
	{
		set_attribute(UIAttribute::Type, type);
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
	ButtonComponent::ButtonComponent()
		: InputComponent(UIInputType::Button)
	{}

	ButtonComponent::ButtonComponent(StringView32 text)
		: InputComponent(UIInputType::Button)
	{
		set_text(text);
	}

	ButtonComponent::ButtonComponent(StringView32 text, const UIAction& action)
		: ButtonComponent(text)
	{
		set_click_action(action);
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