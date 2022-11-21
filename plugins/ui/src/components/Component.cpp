#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	void Component::mount(UIDocument& document, const UINode& parent)
	{
		if (is_mounted()) return;

		_document = &document;
		_parent = parent;

		on_mount();
	}

	void Component::dismount()
	{
		if (!is_mounted()) return;

		on_dismount();

		_document = nullptr;
		_parent = UINode::Empty;
	}
}