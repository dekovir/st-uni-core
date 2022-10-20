#include "unicore/ui/UINode.hpp"

namespace unicore
{
	UINode::UINode(UINodeType type, const Weak<UINode>& parent)
		: _type(type), _parent(parent)
	{
	}

	void UINode::set_attribute(StringView name, const Optional<UIAttributeValue>& value)
	{
		// TODO: Convert to lower
		const auto key = String(name);
		if (value.has_value())
			_attributes[key] = value.value();
		else
		{
			const auto it = _attributes.find(key);
			if (it != _attributes.end())
				_attributes.erase(it);
		}
		// callback
	}

	Optional<UIAttributeValue> UINode::get_attribute(StringView name) const
	{
		// TODO: Convert to lower
		const auto key = String(name);
		const auto it = _attributes.find(key);
		if (it != _attributes.end())
			return it->second;

		return std::nullopt;
	}

	void UINode::set_action(StringView name, const Optional<UINodeActionValue>& value)
	{
		// TODO: Convert to lower
		const auto key = String(name);
		if (value.has_value())
			_actions[key] = value.value();
		else
		{
			const auto it = _actions.find(key);
			if (it != _actions.end())
				_actions.erase(it);
		}
		// callback
	}

	Optional<UINodeActionValue> UINode::get_action(StringView name) const
	{
		// TODO: Convert to lower
		const auto key = String(name);
		const auto it = _actions.find(key);
		if (it != _actions.end())
			return it->second;

		return std::nullopt;
	}

	Shared<UINode> UINode::create_node(UINodeType type)
	{
		auto node = std::make_shared<UINode>(type, shared_from_this());
		_children.push_back(node);
		_event_add_node.invoke(node);
		return node;
	}
}