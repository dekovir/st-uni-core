#include "unicore/ui/elements/ElementContainer.hpp"

namespace unicore::ui
{
	Bool ElementContainer::rebuild_element(const Element& element)
	{
		if (_document)
		{
			if (const auto it = _infos.find(element.index()); it != _infos.end())
				return internal_rebuild(it->second);
		}

		return false;
	}

	void ElementContainer::internal_add(const Shared<Element>& element)
	{
		Info info;
		info.element = element;

		element->set_parent(this);

		_infos[element->index()] = info;
		_indexes.push_back(element->index());
	}

	Bool ElementContainer::internal_rebuild(Info& info)
	{
		const auto temp = info.element->render();
		if (!temp)
		{
			if (!info.node.empty())
			{
				_document->remove_node(info.node);
				info.node = UINode::Empty;
			}
			return true;
		}

		if (!info.node.empty())
		{
			if (temp->apply_to(*_document, info.node))
				return true;

			_document->remove_node(info.node);
			info.node = UINode::Empty;
		}

		// TODO: Rearrange
		info.node = temp->create(*_document, get_container_node());
		return true;
	}
}