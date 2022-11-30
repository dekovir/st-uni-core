#include "unicore/remoteui/View.hpp"

namespace unicore::remoteui
{
	View::View()
		: _bind_create_node([this](auto& node) { on_create_node(node); })
		, _bind_remove_node([this](auto& node) { on_remove_node(node); })
		, _bind_reorder_children([this](auto& node) { on_reorder_children(node); })
		, _bind_set_attribute([this](auto& node, auto type, auto& value) { on_set_attribute(node, type, value); })
	{
	}

	void View::set_document(const Shared<Document>& document)
	{
		if (_document == document)
			return;

		if (_document)
		{
			_document->on_create_node() -= _bind_create_node;
			_document->on_remove_node() -= _bind_remove_node;
			_document->on_reorder_children() -= _bind_reorder_children;
			_document->on_set_attribute() -= _bind_set_attribute;
		}

		_document = document;

		if (_document)
		{
			_document->on_create_node() += _bind_create_node;
			_document->on_remove_node() += _bind_remove_node;
			_document->on_reorder_children() += _bind_reorder_children;
			_document->on_set_attribute() += _bind_set_attribute;
		}

		on_rebuild();
	}
}