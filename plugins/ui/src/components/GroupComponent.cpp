#include "unicore/ui/components/GroupComponent.hpp"

namespace unicore::ui
{
	GroupComponent::GroupComponent(UIGroupType type)
		: Component(UINodeTag::Group)
	{
		set_attribute(UIAttribute::Type, type);
	}

	void GroupComponent::on_mount()
	{
		for (const auto& child : _children)
			child->mount(*document(), node());
	}

	void GroupComponent::on_dismount()
	{
		for (const auto& child : _children)
			child->dismount();
	}
}