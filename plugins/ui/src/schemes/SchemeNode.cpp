#include "unicore/ui/schemes/SchemeNode.hpp"

namespace unicore::ui
{
	SchemeNode::SchemeNode(UINodeTag tag)
		: _tag(tag)
	{}

	UINode SchemeNode::create(UIDocument& document, const UINode& parent) const
	{
		UINodeOptions options;
		fill_optons(options);
		return document.create_node(_tag, options, parent);
	}

	Bool SchemeNode::apply_to(UIDocument& document, const UINode& node) const
	{
		if (!node.empty() && node.tag() == _tag)
		{
			UINodeOptions options;
			fill_optons(options);
			return document.apply_options(node, options);
		}

		return false;
	}
}