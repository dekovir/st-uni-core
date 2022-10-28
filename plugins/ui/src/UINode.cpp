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
			if (const auto value = _document->get_node_type(*this); value.has_value())
				return value.value();
		}

		return UINodeType::Group;
	}

	Optional<UINode> UINode::parent() const
	{
		if (_document)
			return _document->get_node_parent(*this);

		return std::nullopt;
	}

	const String& UINode::uid() const
	{
		static const String s_empty;
		return _document ? _document->get_node_uid(*this) : s_empty;
	}

	const String& UINode::name() const
	{
		static const String s_empty;
		return _document ? _document->get_node_name(*this) : s_empty;
	}

	Bool UINode::visible() const
	{
		return _document ? _document->get_node_visible(*this) : false;
	}

	const UIAttributeDict& UINode::attributes() const
	{
		static const UIAttributeDict s_empty;
		return _document ? _document->get_node_attributes(*this) : s_empty;
	}

	const UIActionDict& UINode::actions() const
	{
		static const UIActionDict s_empty;
		return _document ? _document->get_node_actions(*this) : s_empty;
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
			auto& actions = _document->get_node_actions(*this);
			if (const auto it = actions.find(type); it != actions.end())
				return it->second;
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

	Optional<UINode> UINode::get_next_sibling() const
	{
		return _document ? _document->get_node_next_sibling(*this) : std::nullopt;
	}

	Optional<UINode> UINode::get_prev_sibling() const
	{
		return _document ? _document->get_node_prev_sibling(*this) : std::nullopt;
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

	UNICODE_STRING_BUILDER_FORMAT(const UINode&)
	{
		return builder << "N(" << value.index() << ")";
	}
}