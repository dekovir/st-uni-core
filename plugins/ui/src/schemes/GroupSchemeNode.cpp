#include "unicore/ui/schemes/GroupSchemeNode.hpp"

namespace unicore::ui
{
	GroupSchemeNode::GroupSchemeNode(UIGroupType type)
		: SchemeNode(UINodeTag::Group), _type(type)
	{}

	UINode GroupSchemeNode::create(UIDocument& document, const UINode& parent) const
	{
		const auto node = SchemeNode::create(document, parent);
		for (const auto& child_scheme : _children)
			child_scheme->create(document, node);
		return node;
	}

	Bool GroupSchemeNode::apply_to(UIDocument& document, const UINode& node) const
	{
		if (!SchemeNode::apply_to(document, node))
			return false;

		// TODO: Optimize
		if (document.get_node_children_count(node) != _children.size())
			return false;

		for (unsigned i = 0; i < _children.size(); i++)
		{
			const auto& scheme = _children[i];

			auto child = document.get_node_child(node, i);
			if (child.empty()) return false;

			if (!scheme->apply_to(document, child))
				return false;
		}

		return true;
	}

	void GroupSchemeNode::internal_add(const Shared<SchemeNode>& node)
	{
		_children.push_back(node);
	}

	void GroupSchemeNode::fill_optons(UINodeOptions& options) const
	{
		options.attributes[UIAttribute::Type] = _type;
	}
}