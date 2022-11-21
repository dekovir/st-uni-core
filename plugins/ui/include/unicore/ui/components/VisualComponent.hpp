#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	class VisualComponent : public Component
	{
	public:
		explicit VisualComponent(UIVisualType type)
			: Component(UINodeTag::Visual)
		{
			set_attribute(UIAttribute::Type, type);
		}
	};

	template<UIVisualType Type>
	class VisualComponentType : public VisualComponent
	{
	public:
		VisualComponentType() : VisualComponent(Type) {}
	};

	using separator = VisualComponentType<UIVisualType::Separator>;
	using bullet = VisualComponentType<UIVisualType::Bullet>;
}