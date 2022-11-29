#include "unicore/ui/elements/ElementContainer.hpp"

namespace unicore::ui
{
	void ElementContainer::did_mount()
	{
		Element::did_mount();

		for (const auto& element : _elements)
			element->mount(*document(), node());
	}

	//void ElementContainer::will_umount()
	//{
	//	Element::will_umount();

	//	for (const auto& element : _elements)
	//		element->unmount();
	//}

	void ElementContainer::did_update()
	{
		Element::did_update();

		for (const auto& element : _elements)
			element->update();
	}

	void ElementContainer::internal_add(const Shared<Element>& element)
	{
		_elements.push_back(element);
		rebuild();
	}
}