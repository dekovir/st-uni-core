#include "unicore/ui/UIDocument.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	UIDocument::UIDocument(Logger* logger)
		: _logger(logger)
	{
	}

	size_t UIDocument::get_root_nodes(List<UINode>& nodes)
	{
		List<UINodeIndex> indices;
		get_root_indices(indices);

		for (auto index : indices)
			nodes.push_back(UINode(*this, index));

		return indices.size();
	}

	List<UINode> UIDocument::get_root_nodes()
	{
		List<UINode> nodes;
		get_root_nodes(nodes);
		return nodes;
	}

	UINodeIndex UIDocument::find_index_by_id(StringView id) const
	{
		// TODO: To lower
		const String key(id);
		const auto it = _id_dict.find(key);
		return it != _id_dict.end() ? it->second : UINodeIndexInvalid;
	}

	Optional<UINode> UIDocument::find_node_by_id(StringView id)
	{
		if (const auto index = find_index_by_id(id); index != UINodeIndexInvalid)
			return UINode(*this, index);

		return std::nullopt;
	}

	void UIDocument::send_event(const UIEvent& evt)
	{
		switch (evt.type)
		{
		case UIEventType::ActionCall:
			if (const auto ptr = std::get_if<UIActionType>(&evt.value))
			{
				const auto type = *ptr;
				if (const auto action = evt.node.get_action(type); action.has_value())
					action.value()();
			}
			break;

		case UIEventType::ValueChanged:
			UC_LOG_DEBUG(_logger) << "Value at " << evt.node.index()
				<< " changed to " << std::get<Double>(evt.value);
			break;
		}
	}

	size_t UIDocument::get_root_indices(List<UINodeIndex>& indices) const
	{
		size_t count = 0;
		for (unsigned i = 0; i < _nodes.size(); i++)
		{
			if (_nodes[i].parent == UINodeIndexInvalid)
			{
				indices.push_back(UINodeIndex(i));
				count++;
			}
		}

		return count;
	}

	List<UINodeIndex> UIDocument::get_root_indices() const
	{
		List<UINodeIndex> indices;
		get_root_indices(indices);
		return indices;
	}

	UINodeIndex UIDocument::create_node(UINodeType type, UINodeIndex parent,
		const UIAttributes& attributes, const UINodeActions& actions)
	{
		const auto index = UINodeIndex(_nodes.size());
		_nodes.push_back({});
		auto& info = _nodes.back();

		info.type = type;
		info.parent = parent;
		info.attributes = attributes;
		info.actions = actions;

		if (const auto parent_info = get_info(parent))
			parent_info->children.push_back(index);

		if (const auto it = attributes.find(UIAttributeType::Uid); it != attributes.end())
		{
			String uid;
			if (it->second.try_get_string(uid))
				_id_dict[uid] = index;
		}

		const UINode node(*this, index);
		_event_create_node.invoke(node);

		return index;
	}

	Bool UIDocument::is_node_valid(UINodeIndex index) const
	{
		return get_info(index) != nullptr;
	}

	UINodeType UIDocument::get_node_type(UINodeIndex index) const
	{
		const auto info = get_info(index);
		return info ? info->type : UINodeType::None;
	}

	UINodeIndex UIDocument::get_node_parent(UINodeIndex index) const
	{
		const auto info = get_info(index);
		return info ? info->parent : UINodeIndexInvalid;
	}

	const List<UINodeIndex>& UIDocument::get_node_children(UINodeIndex index) const
	{
		static const List<UINodeIndex> s_empty{};

		const auto info = get_info(index);
		return info ? info->children : s_empty;
	}

	UINodeIndex UIDocument::get_node_next_sibling(UINodeIndex index) const
	{
		if (const auto info = get_info(index))
		{
			if (const auto parent_info = get_info(info->parent))
			{
				const auto it = std::find(
					parent_info->children.begin(),
					parent_info->children.end(), index);
				if (it != parent_info->children.end())
				{
					const auto i = it - parent_info->children.end();
					if (i + 1 < parent_info->children.size())
						return UINodeIndex(i + 1);
				}
			}
		}

		return UINodeIndexInvalid;
	}

	UINodeIndex UIDocument::get_node_prev_sibling(UINodeIndex index) const
	{
		if (const auto info = get_info(index))
		{
			if (const auto parent_info = get_info(info->parent))
			{
				const auto it = std::find(
					parent_info->children.begin(),
					parent_info->children.end(), index);
				if (it != parent_info->children.end())
				{
					const auto i = it - parent_info->children.end();
					if (i > 0)
						return UINodeIndex(i - 1);
				}
			}
		}

		return UINodeIndexInvalid;
	}

	void UIDocument::set_node_attribute(UINodeIndex index,
		UIAttributeType type, const Optional<Variant>& value)
	{
		if (const auto info = get_info(index))
		{
			if (type == UIAttributeType::Uid && value.has_value())
			{
				String id;
				if (value.value().try_get_string(id))
					_id_dict[id] = index;
			}

			if (value.has_value())
				info->attributes[type] = value.value();
			else
			{
				if (const auto it = info->attributes.find(type); it != info->attributes.end())
					info->attributes.erase(it);
				else return;
			}

			if (!_event_set_attribute.empty())
			{
				const UINode node(*this, index);
				_event_set_attribute.invoke(node, type, value);
			}
		}
	}

	Optional<Variant> UIDocument::get_node_attribute(
		UINodeIndex index, UIAttributeType type) const
	{
		if (const auto info = get_info(index))
		{
			if (const auto it = info->attributes.find(type); it != info->attributes.end())
				return it->second;
		}
		return std::nullopt;
	}

	const UIAttributes& UIDocument::get_node_attributes(UINodeIndex index) const
	{
		static const UIAttributes s_empty{};
		const auto info = get_info(index);
		return info ? info->attributes : s_empty;
	}

	void UIDocument::set_node_action(UINodeIndex index,
		UIActionType type, const Optional<UIAction>& action)
	{
		if (const auto info = get_info(index))
		{
			if (action.has_value())
			{
				info->actions[type] = action.value();
			}
			else
			{
				if (const auto it = info->actions.find(type); it != info->actions.end())
					info->actions.erase(it);
				else return;
			}

			if (!_event_set_action.empty())
			{
				const UINode node(*this, index);
				_event_set_action.invoke(node, type, action);
			}
		}
	}

	const UINodeActions& UIDocument::get_node_actions(UINodeIndex index) const
	{
		static const UINodeActions s_empty{};
		const auto info = get_info(index);
		return info ? info->actions : s_empty;
	}

	UIDocument::NodeInfo* UIDocument::get_info(UINodeIndex index)
	{
		if (index != UINodeIndexInvalid)
			return &_nodes[index.value];

		return nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(UINodeIndex index) const
	{
		if (index != UINodeIndexInvalid)
			return &_nodes[index.value];

		return nullptr;
	}
}