#include "unicore/ui/UINode.hpp"
#include "unicore/ui/UIDocument.hpp"
#include "unicore/system/Unicode.hpp"

namespace unicore
{
	UINode::UINode(UIDocument& document, const UINodeIndex& index)
		: _document(document)
		, _index(index)
	{
	}

	Bool UINode::valid() const
	{
		return _document.is_node_valid(_index);
	}

	UINodeType UINode::type() const
	{
		return _document.get_node_type(_index);
	}

	Optional<UINode> UINode::parent() const
	{
		const auto parent = _document.get_node_parent(_index);
		if (parent != UINodeIndexInvalid)
			return UINode(_document, parent);
		return std::nullopt;
	}

	const UIAttributes& UINode::attributes() const
	{
		return _document.get_node_attributes(_index);
	}

	const UINodeActions& UINode::actions() const
	{
		return _document.get_node_actions(_index);
	}

	void UINode::set_attribute(UIAttributeType type, const Optional<UIAttributeValue>& value)
	{
		_document.set_node_attribute(_index, type, value);
	}

	Optional<UIAttributeValue> UINode::get_attribute(UIAttributeType type) const
	{
		return _document.get_node_attribute(_index, type);
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
		auto& indices = _document.get_node_children(_index);
		if (!indices.empty())
		{
			for (auto index : indices)
				children.push_back(UINode(_document, index));
			return indices.size();
		}

		return 0;
	}

	List<UINode> UINode::get_children() const
	{
		List<UINode> list;
		get_children(list);
		return list;
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

	UINode UINode::create_child(UINodeType type)
	{
		const auto child_index = _document.create_node(type, _index);
		return { _document, child_index };
	}

	UINode UINode::create_sibling(UINodeType type)
	{
		const auto parent = _document.get_node_parent(_index);
		const auto child_index = _document.create_node(type, parent);
		return { _document, child_index };
	}

	bool UINode::try_get_float(UIAttributeType type, Float& value) const
	{
		if (const auto result = get_attribute(type); result.has_value())
		{
			if (const auto ptr = std::get_if<Int>(&result.value()))
			{
				value = *ptr;
				return true;
			}

			if (const auto ptr = std::get_if<Int64>(&result.value()))
			{
				value = static_cast<Float>(*ptr);
				return true;
			}

			if (const auto ptr = std::get_if<Float>(&result.value()))
			{
				value = *ptr;
				return true;
			}

			if (const auto ptr = std::get_if<Double>(&result.value()))
			{
				value = static_cast<Float>(*ptr);
				return true;
			}
		}

		return false;
	}

	Float UINode::get_float(UIAttributeType type, Float default_value) const
	{
		Float value;
		return try_get_float(type, value) ? value : default_value;
	}

	bool UINode::try_get_double(UIAttributeType type, Double& value) const
	{
		if (const auto result = get_attribute(type); result.has_value())
		{
			if (const auto ptr = std::get_if<Int>(&result.value()))
			{
				value = *ptr;
				return true;
			}

			if (const auto ptr = std::get_if<Int64>(&result.value()))
			{
				value = static_cast<Float>(*ptr);
				return true;
			}

			if (const auto ptr = std::get_if<Float>(&result.value()))
			{
				value = *ptr;
				return true;
			}

			if (const auto ptr = std::get_if<Double>(&result.value()))
			{
				value = *ptr;
				return true;
			}
		}

		return false;
	}

	Double UINode::get_double(UIAttributeType type, Double default_value) const
	{
		Double value;
		return try_get_double(type, value) ? value : default_value;
	}

	bool UINode::try_get_string(UIAttributeType type, String& value) const
	{
		if (const auto result = get_attribute(type); result.has_value())
		{
			if (const auto str = std::get_if<String>(&result.value()))
			{
				value = *str;
				return true;
			}

			if (const auto str = std::get_if<String32>(&result.value()))
			{
				value = Unicode::to_utf8(*str);
				return true;
			}
		}

		return false;
	}

	String UINode::get_string(UIAttributeType type, StringView default_value) const
	{
		String str;
		return try_get_string(type, str) ? str : String(default_value);
	}

	bool UINode::try_get_string32(UIAttributeType type, String32& value) const
	{
		if (const auto result = get_attribute(type); result.has_value())
		{
			if (const auto str = std::get_if<String32>(&result.value()))
			{
				value = *str;
				return true;
			}

			if (const auto str = std::get_if<String>(&result.value()))
			{
				value = Unicode::to_utf32(*str);
				return true;
			}
		}

		return false;
	}

	String32 UINode::get_string32(UIAttributeType type, StringView32 default_value) const
	{
		String32 str;
		return try_get_string32(type, str) ? str : String32(default_value);
	}

	UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&)
	{
		return builder << "I(" << value.value << ")";
	}
}