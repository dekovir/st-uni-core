#include "unicore/ui/UIView.hpp"

namespace unicore
{
	UIView::UIView()
		: _bind_create_node([this](auto& node) {on_create_node(node); })
		, _bind_set_attribute([this](auto& node, auto type, auto& value) { on_set_attribute(node, type, value); })
		, _bind_set_action([this](auto& node, auto type, auto& value) { on_set_action(node, type, value); })
	{
	}

	void UIView::set_document(const Shared<UIDocument>& document)
	{
		if (_document == document)
			return;

		if (_document)
		{
			_document->on_create_node() -= _bind_create_node;
			_document->on_set_attribute() -= _bind_set_attribute;
			_document->on_set_action() -= _bind_set_action;
		}

		_document = document;

		if (_document)
		{
			_document->on_create_node() += _bind_create_node;
			_document->on_set_attribute() += _bind_set_attribute;
			_document->on_set_action() += _bind_set_action;
		}

		on_rebuild();
	}

	UIGroupValue UIView::get_group_value(const Variant& value, UIGroupValue default_value)
	{
		UIGroupValue layout;
		return value.try_get_enum(layout) ? layout : default_value;
	}
}