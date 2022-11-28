#include "unicore/ui/elements/ElementContainer.hpp"

namespace unicore::ui
{
	void ElementContainer::did_update()
	{
		Element::did_update();

		for (const auto& element : _elements)
			element->update();
	}

	void ElementContainer::internal_add(const Shared<Element>& element)
	{
		_elements.push_back(element);
	}
}