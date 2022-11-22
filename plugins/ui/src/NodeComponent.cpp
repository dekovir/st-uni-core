#include "unicore/ui/components/NodeComponent.hpp"

namespace unicore::ui
{
	NodeComponent::NodeComponent(UINodeTag tag)
		: _tag(tag)
	{
	}

	void NodeComponent::set_attribute(UIAttribute type, const Variant& value)
	{
		_options.attributes[type] = value;
		if (is_mounted())
			document()->set_node_attribute(_node, type, value);
	}

	//void NodeComponent::set_action(UIActionType type, const UIAction& action)
	//{
	//	_options.actions[type] = action;
	//	if (is_mounted())
	//		document()->subscribe_node(_node, type, action);
	//}

	void NodeComponent::on_mount()
	{
		apply_options(_options);

		_node = document()->create_node(_tag, _options, parent());
	}

	void NodeComponent::on_dismount()
	{
		document()->remove_node(_node);
	}
}