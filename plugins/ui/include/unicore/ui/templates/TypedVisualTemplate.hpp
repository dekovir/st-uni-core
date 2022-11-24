#pragma once
#include "unicore/ui/templates/TypedTemplate.hpp"

namespace unicore::ui
{
	template<UIVisualType Type, typename... TKeys>
	class TypedVisualTemplate : public TypedTemplate<UINodeTag::Visual, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Visual, TKeys...>;

		TypedVisualTemplate()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedVisualTemplate(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);
		}
	};

	template<UIVisualType Type, typename ... TValues>
	using VisualTemplate = TypedVisualTemplate<Type, TValues...>;

	using VisualText = VisualTemplate<UIVisualType::Text, attr::Text>;
	using VisualColor = VisualTemplate<UIVisualType::Color, attr::Value>;
	using VisualImage = VisualTemplate<UIVisualType::Image, attr::Value>;
	using VisualProgress = VisualTemplate<UIVisualType::Progress, attr::Value, attr::Min, attr::Max>;
	using VisualSeparator = VisualTemplate<UIVisualType::Separator>;
	using VisualBullet = VisualTemplate<UIVisualType::Bullet>;
}