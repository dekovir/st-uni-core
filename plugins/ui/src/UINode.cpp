#include "unicore/ui/UINode.hpp"
#include "unicore/ui/UIDocument.hpp"

namespace unicore
{
	const UINode UINode::Empty;

	Bool UINode::empty() const
	{
		return _document == nullptr || _index == UINodeIndex_Invalid;
	}

	Bool UINode::valid() const
	{
		return _document && _document->is_node_valid(*this);
	}

	UINodeTag UINode::tag() const
	{
		if (_document)
		{
			UINodeTag type;
			if (_document->get_node_tag(*this, type))
				return type;
		}

		return UINodeTag::Group;
	}

	UINode UINode::parent() const
	{
		return _document ? _document->get_node_parent(*this) : Empty;
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

	Variant UINode::get(UIAttribute attribute) const
	{
		return _document ? _document->get_node_attribute(*this, attribute) : Variant::Empty;
	}

	Bool UINode::has(UIAttribute attribute) const
	{
		return _document ? _document->get_node_attribute(*this, attribute) != Variant::Empty : false;
	}

	StringView UINode::name() const
	{
		if (_document)
			return _document->get_node_name(*this);

		return {};
	}

	size_t UINode::get_children(List<UINode>& children) const
	{
		return _document ? _document->get_node_children(children, *this) : 0;
	}

	List<UINode> UINode::get_children() const
	{
		if (_document)
			return _document->get_node_children(*this);
		return  {};
	}

	Size UINode::get_children_count() const
	{
		return _document ? _document->get_node_children_count(*this) : 0;
	}

	int UINode::get_sibling_index() const
	{
		if (_document)
			return _document->get_node_sibling_index(*this);

		return -1;
	}

	UINode UINode::get_next_sibling() const
	{
		return _document ? _document->get_node_next_sibling(*this) : Empty;
	}

	UINode UINode::get_prev_sibling() const
	{
		return _document ? _document->get_node_prev_sibling(*this) : Empty;
	}

	// FIND //////////////////////////////////////////////////////////////////////
	UINode UINode::find_by_type(UINodeTag tag) const
	{
		return _document ? _document->find_by_tag(tag, *this) : Empty;
	}

	Size UINode::find_all_by_type(UINodeTag tag, List<UINode>& list) const
	{
		return _document ? _document->find_all_by_tag(tag, list, *this) : 0;
	}

	UINode UINode::find_by_name(StringView name) const
	{
		return _document ? _document->find_by_name(name, *this) : Empty;
	}

	Size UINode::find_all_by_name(StringView name, List<UINode>& list) const
	{
		return _document ? _document->find_all_by_name(name, list, *this) : 0;
	}

	UINode UINode::query(const Predicate<const UINode&>& predicate) const
	{
		return _document ? _document->query(predicate, *this) : Empty;
	}

	Size UINode::query_all(const Predicate<const UINode&>& predicate, List<UINode>& list) const
	{
		return _document ? _document->query_all(predicate, list, *this) : 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINode&)
	{
		return builder << "N(" << value.tag() << ":" << value.index() << ")";
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&)
	{
		return builder << "I" << value.value;
	}
}