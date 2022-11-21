#include "unicore/ui/components/VisualComponent.hpp"

namespace unicore::ui
{
	VisualComponent::VisualComponent(UIVisualType type)
		: NodeComponent(UINodeTag::Visual)
	{
		set_attribute(UIAttribute::Type, type);
	}
}