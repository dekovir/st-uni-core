#include "unicore/ui/UINode.hpp"
#include "unicore/ui/UIDocument.hpp"

namespace unicore
{
	static List<UINodeIndex> s_indices;
	static List<UINode> s_nodes;

	UINode::UINode(UIDocument& document, const UINodeIndex& index)
		: _document(document)
		, _index(index)
	{
	}

	UINode& UINode::operator=(UINode const& other) noexcept
	{
		_document = other._document;
		_index = other._index;
		return *this;
	}

	UINode& UINode::operator=(UINode&& other) noexcept
	{
		_document = other._document;
		_index = other._index;
		return *this;
	}

	Bool UINode::valid() const
	{
		return _document.is_node_valid(_index);
	}

	UINodeType UINode::type() const
	{
		const auto value = _document.get_node_type(_index);
		return value.has_value() ? value.value() : UINodeType::Group;
	}

	Optional<UINode> UINode::parent() const
	{
		const auto parent = _document.get_node_parent(_index);
		if (parent != UINodeIndexInvalid)
			return UINode(_document, parent);
		return std::nullopt;
	}

	const String& UINode::uid() const
	{
		return _document.get_node_uid(_index);
	}

	const String& UINode::name() const
	{
		return _document.get_node_name(_index);
	}

	void UINode::set_name(StringView name)
	{
		_document.set_node_name(_index, name);
	}

	Bool UINode::visible() const
	{
		return _document.get_node_visible(_index);
	}

	void UINode::set_visible(Bool value)
	{
		_document.set_node_visible(_index, value);
	}

	const UIAttributeDict& UINode::attributes() const
	{
		return _document.get_node_attributes(_index);
	}

	const UIActionDict& UINode::actions() const
	{
		return _document.get_node_actions(_index);
	}

	void UINode::set_attribute(UIAttributeType type, const Optional<Variant>& value)
	{
		_document.set_node_attribute(_index, type, value);
	}

	Variant UINode::get_attribute(UIAttributeType type) const
	{
		const auto result = _document.get_node_attribute(_index, type);
		return result.has_value() ? result.value() : Variant::Empty;
	}

	void UINode::set_action(UIActionType type, const Optional<UIAction>& value)
	{
		_document.set_node_action(_index, type, value);
	}

	Optional<UIAction> UINode::get_action(UIActionType type) const
	{
		auto& actions = _document.get_node_actions(_index);
		if (const auto it = actions.find(type); it != actions.end())
			return it->second;

		return std::nullopt;
	}

	size_t UINode::get_children(List<UINode>& children) const
	{
		if (auto& indices = _document.get_node_children(_index); !indices.empty())
		{
			for (auto index : indices)
				children.push_back(UINode(_document, index));
			return indices.size();
		}

		return 0;
	}

	List<UINode> UINode::get_children() const
	{
		s_nodes.clear();
		get_children(s_nodes);
		return s_nodes;
	}

	Optional<UINode> UINode::get_next_sibling() const
	{
		const auto next_index = _document.get_node_next_sibling(_index);
		if (next_index != UINodeIndexInvalid)
			return UINode(_document, next_index);
		return std::nullopt;
	}

	Optional<UINode> UINode::get_prev_sibling() const
	{
		const auto prev_index = _document.get_node_prev_sibling(_index);
		if (prev_index != UINodeIndexInvalid)
			return UINode(_document, prev_index);
		return std::nullopt;
	}

	UINode UINode::create_child(UINodeType type, const UINodeOptions& options)
	{
		const auto child_index = _document.create_node(type, _index, options);
		return { _document, child_index };
	}

	UINode UINode::create_sibling(UINodeType type, const UINodeOptions& options)
	{
		const auto parent = _document.get_node_parent(_index);
		const auto child_index = _document.create_node(type, parent, options);
		return { _document, child_index };
	}

	UINode UINode::duplicate()
	{
		const auto new_index = _document.duplicate_node(_index);
		return { _document, new_index };
	}

	UINode UINode::duplicate_at(UINodeIndex parent)
	{
		const auto new_index = _document.duplicate_node_at(_index, parent);
		return { _document, new_index };
	}

	Optional<UINode> UINode::find_child_by_name(StringView name) const
	{
		return _document.find_node_by_name(name, _index);
	}

	Optional<UINode> UINode::find_child_by_name_recurse(StringView name) const
	{
		return _document.find_node_by_name_recurse(name, _index);
	}

	size_t UINode::find_childs_by_name(StringView name, List<UINode>& list) const
	{
		return _document.find_nodes_by_name(name, list, _index);
	}

	size_t UINode::find_childs_by_name_recurse(StringView name, List<UINode>& list) const
	{
		return _document.find_nodes_by_name_recurse(name, list, _index);
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&)
	{
		return builder << "I(" << value.value << ")";
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINode&)
	{
		return builder << "N(" << value.index().value << ")";
	}
}