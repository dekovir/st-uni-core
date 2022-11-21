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
}