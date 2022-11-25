#include "unicore/ui/schemes/InputSchemeNode.hpp"

namespace unicore::ui
{
	InputSchemeNode::InputSchemeNode(UIInputType type)
		: SchemeNode(UINodeTag::Input), _type(type)
	{}

	void InputSchemeNode::fill_optons(UINodeOptions& options) const
	{
		options.attributes[UIAttribute::Type] = _type;
	}
}