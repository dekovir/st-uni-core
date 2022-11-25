#pragma once
#include "unicore/ui/schemes/TypedNodeScheme.hpp"

namespace unicore::ui
{
	template<UIVisualType Type, typename... TKeys>
	class TypedVisualScheme : public TypedNodeScheme<UINodeTag::Visual, TKeys...>
	{
	public:
		using BaseClass = TypedNodeScheme<UINodeTag::Visual, TKeys...>;

		TypedVisualScheme()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedVisualScheme(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);
		}
	};

	template<UIVisualType Type, typename ... TValues>
	using VisualTemplate = TypedVisualScheme<Type, TValues...>;

	using VisualText = VisualTemplate<UIVisualType::Text, attr::Text>;
	using VisualColor = VisualTemplate<UIVisualType::Color, attr::Value>;
	using VisualImage = VisualTemplate<UIVisualType::Image, attr::Value>;
	using VisualProgress = VisualTemplate<UIVisualType::Progress, attr::Value, attr::Min, attr::Max>;
	using VisualSeparator = VisualTemplate<UIVisualType::Separator>;
	using VisualBullet = VisualTemplate<UIVisualType::Bullet>;
}