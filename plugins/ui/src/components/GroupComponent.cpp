#include "unicore/ui/components/GroupComponent.hpp"

namespace unicore::ui
{
	GroupComponent::GroupComponent(UIGroupType type)
		: NodeComponent(UINodeTag::Group)
	{
		set_attribute(UIAttribute::Type, type);
	}

	void GroupComponent::on_mount()
	{
		NodeComponent::on_mount();

		for (const auto& child : _children)
			child->mount(*document(), node());
	}

	void GroupComponent::on_dismount()
	{
		for (const auto& child : _children)
			child->dismount();

		NodeComponent::on_dismount();
	}

	void GroupComponent::internal_add(const Shared<Component>& component)
	{
		_children.push_back(component);
		if (is_mounted())
			component->mount(*document(), node());
	}
}