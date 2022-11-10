#include "unicore/ui/UIView.hpp"

namespace unicore
{
	UIView::UIView()
		: _bind_create_node([this](auto& node) { on_create_node(node); })
		, _bind_remove_node([this](auto& node) { on_remove_node(node); })
		, _bind_set_name([this](auto& node, auto value) { on_set_name(node, value); })
		, _bind_set_style([this](auto& node, auto value) { on_set_style(node, value); })
		, _bind_set_visible([this](auto& node, auto value) { on_set_visible(node, value); })
		, _bind_set_attribute([this](auto& node, auto type, auto& value) { on_set_attribute(node, type, value); })
	{
	}

	void UIView::set_document(const Shared<UIDocument>& document)
	{
		if (_document == document)
			return;

		if (_document)
		{
			_document->on_create_node() -= _bind_create_node;
			_document->on_remove_node() -= _bind_remove_node;
			_document->on_set_name() -= _bind_set_name;
			_document->on_set_style() -= _bind_set_style;
			_document->on_set_visible() -= _bind_set_visible;
			_document->on_set_attribute() -= _bind_set_attribute;
		}

		_document = document;

		if (_document)
		{
			_document->on_create_node() += _bind_create_node;
			_document->on_remove_node() += _bind_remove_node;
			_document->on_set_name() += _bind_set_name;
			_document->on_set_style() += _bind_set_style;
			_document->on_set_visible() += _bind_set_visible;
			_document->on_set_attribute() += _bind_set_attribute;
		}

		on_rebuild();
	}
}