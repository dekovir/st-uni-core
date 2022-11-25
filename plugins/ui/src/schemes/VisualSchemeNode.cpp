#include "unicore/ui/schemes/VisualSchemeNode.hpp"

namespace unicore::ui
{
	VisualSchemeNode::VisualSchemeNode(UIVisualType type)
		: SchemeNode(UINodeTag::Visual), _type(type)
	{}

	void VisualSchemeNode::fill_optons(UINodeOptions& options) const
	{
		options.attributes[UIAttribute::Type] = _type;
	}
}