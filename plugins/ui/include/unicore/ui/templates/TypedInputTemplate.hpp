#pragma once
#include "unicore/ui/templates/TypedTemplate.hpp"

namespace unicore::ui
{
	template<UIInputType Type, typename... TKeys>
	class TypedInputTemplate : public TypedTemplate<UINodeTag::Input, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Input, TKeys...>;

		TypedInputTemplate()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedInputTemplate(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);
		}
	};

	template<UIInputType Type, typename ... TValues>
	using InputTemplate = TypedInputTemplate<Type, TValues...>;

	using InputText = InputTemplate<UIInputType::Text,
		attr::Value, action::OnChange>;
	using InputTextArea = InputTemplate<UIInputType::TextArea,
		attr::Value, action::OnChange>;

	using InputToggle = InputTemplate<UIInputType::Toggle,
		attr::Value, action::OnChange>;
	using InputRadio = InputTemplate<UIInputType::Radio,
		attr::Value, action::OnChange>;

	using InputButton = InputTemplate<UIInputType::Button,
		attr::Text, action::OnClick>;
	using InputItem = InputTemplate<UIInputType::Item,
		attr::Value, attr::Text, action::OnClick, action::OnChange>;

	using InputImage = InputTemplate<UIInputType::Image,
		attr::Value, action::OnChange>;

	using InputInteger = InputTemplate<UIInputType::Integer,
		attr::Value, attr::Step, attr::Min, attr::Max, action::OnChange>;
	using InputFloat = InputTemplate<UIInputType::Float,
		attr::Value, attr::Step, attr::Min, attr::Max, action::OnChange>;

	using InputSliderI = InputTemplate<UIInputType::RangeI,
		attr::Value, attr::Min, attr::Max, action::OnChange>;
	using InputSliderF = InputTemplate<UIInputType::RangeF,
		attr::Value, attr::Min, attr::Max, action::OnChange>;

	using InputColor3 = InputTemplate<UIInputType::Color3,
		attr::Value, action::OnChange>;
	using InputColor4 = InputTemplate<UIInputType::Color4,
		attr::Value, action::OnChange>;
}