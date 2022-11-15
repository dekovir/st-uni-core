#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	Component::Component(UINodeTag tag)
		: _tag(tag)
	{
	}

	void Component::mount(UIDocument& document, const UINode& parent)
	{
		if (is_mounted()) return;

		_document = &document;
		_node = _document->create_node(_tag, _options, parent);

		on_mount();
	}

	void Component::dismount()
	{
		if (!is_mounted()) return;

		on_dismount();

		_document->remove_node(_node);
		_document = nullptr;
		_node = UINode::Empty;
	}

	void Component::set_attribute(UIAttribute type, const Variant& value)
	{
		_options.attributes[type] = value;
		if (_document)
			_document->set_node_attribute(_node, type, value);
	}

	void Component::set_action(UIActionType type, const UIAction& action)
	{
		_options.actions[type] = action;
		if (_document)
			_document->subscribe_node(_node, type, action);
	}
}