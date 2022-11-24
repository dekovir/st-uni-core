#pragma once
#include "unicore/ui/templates/TypedTemplate.hpp"

namespace unicore::ui
{
	template<UIInputType Type, typename... TKeys>
	class TypedInputTemplate : public TypedTemplate<UINodeTag::Input, attr::Value, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Input, attr::Value, TKeys...>;

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

	using ButtonTemplate = InputTemplate<UIInputType::Button, attr::Text, action::OnClick>;
	using SliderTemplate = InputTemplate<UIInputType::RangeF, attr::Min, attr::Max, action::OnChange>;
}