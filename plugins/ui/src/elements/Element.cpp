#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	static ElementIndex::TypeValue s_last_index = 0;

	Element::Element()
		: _index(s_last_index++)
	{
	}

	void Element::mount(UIDocument& document, const UINode& parent)
	{
		if (is_mounted()) return;

		const auto scheme = render();

		_document = &document;
		_node = scheme->create(*_document, parent);

		did_mount();
	}

	void Element::unmount()
	{
		if (is_mounted())
		{
			will_umount();

			_document->remove_node(_node);

			_document = nullptr;
			_node = UINode::Empty;
		}
	}

	void Element::update()
	{
		if (is_mounted())
		{
			if (_need_rerender)
			{
				_need_rerender = false;

				const auto scheme = render();
				if (!scheme->apply_to(*_document, _node))
				{
					const auto parent = _node.parent();
					_document->remove_node(_node);
					_node = scheme->create(*_document, parent);
				}
			}

			did_update();
		}
	}

	void Element::rebuild()
	{
		_need_rerender = true;
	}
}