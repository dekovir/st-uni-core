#include "unicore/remoteui/Element.hpp"
#include "unicore/remoteui/Document.hpp"

namespace unicore::remoteui
{
	const Element Element::Empty;

	Bool Element::empty() const
	{
		return _document == nullptr || _index == ElementIndex_Invalid;
	}

	Bool Element::valid() const
	{
		return _document && _document->is_node_valid(*this);
	}

	ElementTag Element::tag() const
	{
		if (_document)
		{
			ElementTag type;
			if (_document->get_node_tag(*this, type))
				return type;
		}

		return ElementTag::Group;
	}

	Element Element::parent() const
	{
		return _document ? _document->get_node_parent(*this) : Empty;
	}

	AttributeDict Element::get_attributes() const
	{
		if (_document)
		{
			if (const auto result = _document->get_node_attributes(*this); result.has_value())
				return result.value();
		}

		return {};
	}

	Variant Element::get(Attribute attribute) const
	{
		return _document ? _document->get_node_attribute(*this, attribute) : Variant::Empty;
	}

	Bool Element::has(Attribute attribute) const
	{
		return _document ? _document->get_node_attribute(*this, attribute) != Variant::Empty : false;
	}

	StringView Element::name() const
	{
		if (_document)
			return _document->get_node_name(*this);

		return {};
	}

	size_t Element::get_children(List<Element>& children) const
	{
		return _document ? _document->get_node_children(children, *this) : 0;
	}

	List<Element> Element::get_children() const
	{
		if (_document)
			return _document->get_node_children(*this);
		return  {};
	}

	Size Element::get_children_count() const
	{
		return _document ? _document->get_node_children_count(*this) : 0;
	}

	Element Element::get_children(int index) const
	{
		return _document ? _document->get_node_child(*this, index) : Empty;
	}

	int Element::get_sibling_index() const
	{
		if (_document)
			return _document->get_node_sibling_index(*this);

		return -1;
	}

	Element Element::get_next_sibling() const
	{
		return _document ? _document->get_node_next_sibling(*this) : Empty;
	}

	Element Element::get_prev_sibling() const
	{
		return _document ? _document->get_node_prev_sibling(*this) : Empty;
	}

	// FIND //////////////////////////////////////////////////////////////////////
	Element Element::find_by_type(ElementTag tag) const
	{
		return _document ? _document->find_by_tag(tag, *this) : Empty;
	}

	Size Element::find_all_by_type(ElementTag tag, List<Element>& list) const
	{
		return _document ? _document->find_all_by_tag(tag, list, *this) : 0;
	}

	Element Element::find_by_name(StringView name) const
	{
		return _document ? _document->find_by_name(name, *this) : Empty;
	}

	Size Element::find_all_by_name(StringView name, List<Element>& list) const
	{
		return _document ? _document->find_all_by_name(name, list, *this) : 0;
	}

	Element Element::query(const Predicate<const Element&>& predicate) const
	{
		return _document ? _document->query(predicate, *this) : Empty;
	}

	Size Element::query_all(const Predicate<const Element&>& predicate, List<Element>& list) const
	{
		return _document ? _document->query_all(predicate, list, *this) : 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Element&)
	{
		return builder << "N(" << value.tag() << ":" << value.index() << ")";
	}

	UNICODE_STRING_BUILDER_FORMAT(const ElementIndex&)
	{
		return builder << "I" << value.value;
	}
}