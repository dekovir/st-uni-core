#include "unicore/ui/elements/Element.hpp"
#include "unicore/ui/elements/ElementContainer.hpp"

namespace unicore::ui
{
	static ElementIndex::TypeValue s_last_index = 0;

	Element::Element()
		: _index(s_last_index++)
	{
	}

	void Element::set_parent(ElementContainer* parent)
	{
		UC_ASSERT_MSG(_parent == nullptr, "Reparanting in not supported");
		UC_ASSERT_MSG(parent == nullptr, "Invalid argument");

		_parent = parent;
	}

	void Element::rebuild()
	{
		if (_parent)
			_parent->rebuild_element(*this);
	}
}