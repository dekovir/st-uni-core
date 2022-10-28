#include "unicore/ui/UINode.hpp"
#include "unicore/ui/UIDocument.hpp"

namespace unicore
{
	static List<UINode> s_nodes;

	Bool UINode::valid() const
	{
		return _document && _document->is_node_valid(*this);
	}

	UINodeType UINode::type() const
	{
		if (_document)
		{
			UINodeType type;
			if (_document->get_node_type(*this, type))
				return type;
		}

		return UINodeType::Group;
	}

	UINode UINode::parent() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_parent(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	Optional<String> UINode::uid() const
	{
		String value;
		if (_document && _document->get_node_uid(*this, value))
			return value;

		return std::nullopt;
	}

	Optional<String> UINode::name() const
	{
		String value;
		if (_document && _document->get_node_name(*this, value))
			return value;

		return std::nullopt;
	}

	Optional<String> UINode::style() const
	{
		String value;
		if (_document && _document->get_node_style(*this, value))
			return value;

		return std::nullopt;
	}

	Bool UINode::visible() const
	{
		Bool value;
		if (_document && _document->get_node_visible(*this, value))
			return value;

		return false;
	}

	UIAttributeDict UINode::get_attributes() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_attributes(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	UIActionDict UINode::get_actions() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_actions(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	Variant UINode::attribute(UIAttributeType type) const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_attribute(*this, type); result.has_value())
				return result.value();
		}

		return Variant::Empty;
	}

	Optional<UIAction> UINode::action(UIActionType type) const
	{
		if (_document)
		{
			if (const auto action = _document->get_node_action(*this, type); action.has_value())
				return action.value();
		}

		return std::nullopt;
	}

	size_t UINode::get_children(List<UINode>& children) const
	{
		return _document ? _document->get_node_children(children, *this) : 0;
	}

	List<UINode> UINode::get_children() const
	{
		s_nodes.clear();
		get_children(s_nodes);
		return s_nodes;
	}

	Size UINode::get_children_count() const
	{
		return _document ? _document->get_node_children_count(*this) : 0;
	}

	unsigned UINode::get_sibling_index() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_sibling_index(*this); result.has_value())
				return result.value();
		}

		return 0;
	}

	UINode UINode::get_next_sibling() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_next_sibling(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	UINode UINode::get_prev_sibling() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_prev_sibling(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	// FIND //////////////////////////////////////////////////////////////////////
	UINode UINode::find_by_type(UINodeType type) const
	{
		if (_document)
		{
			if (const auto result = _document->find_by_type(type, *this); result.has_value())
				return result.value();
		}

		return {};
	}

	Size UINode::find_all_by_type(UINodeType type, List<UINode>& list) const
	{
		if (_document)
			return _document->find_all_by_type(type, list, *this);
		return 0;
	}

	UINode UINode::find_by_name(StringView name) const
	{
		if (_document)
		{
			if (const auto result = _document->find_by_name(name, *this); result.has_value())
				return result.value();
		}

		return {};
	}

	Size UINode::find_all_by_name(StringView name, List<UINode>& list) const
	{
		if (_document)
			return _document->find_all_by_name(name, list, *this);

		return 0;
	}

	UINode UINode::querry(const Predicate<const UINode&>& predicate) const
	{
		if (_document)
		{
			if (const auto result = _document->querry(predicate, *this); result.has_value())
				return result.value();
		}

		return {};
	}

	Size UINode::querry_all(const Predicate<const UINode&>& predicate, List<UINode>& list) const
	{
		if (_document)
			return _document->querry_all(predicate, list, *this);

		return 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINode&)
	{
		return builder << "N(" << value.type() << ":" << value.index() << ")";
	}
}