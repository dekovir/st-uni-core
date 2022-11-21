#pragma once
#include "unicore/ui/components/NodeComponent.hpp"

namespace unicore::ui
{
	class VisualComponent : public NodeComponent
	{
	public:
		explicit VisualComponent(UIVisualType type);
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