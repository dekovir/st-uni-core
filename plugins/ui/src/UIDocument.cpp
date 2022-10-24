#include "unicore/ui/UIDocument.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	UIDocument::UIDocument(Logger* logger)
		: _logger(logger)
	{
	}

	Size UIDocument::get_root_nodes(List<UINode>& nodes)
	{
		for (auto index : _roots)
			nodes.push_back(UINode(*this, index));

		return _roots.size();
	}

	List<UINode> UIDocument::get_root_nodes()
	{
		List<UINode> nodes;
		get_root_nodes(nodes);
		return nodes;
	}

	// FIND //////////////////////////////////////////////////////////////////////
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

	Size UIDocument::find_indexes_by_name(StringView name,
		List<UINodeIndex>& list, UINodeIndex parent) const
	{
		Size count = 0;
		if (parent == UINodeIndexInvalid)
		{
			for (unsigned i = 0; i < _nodes.size(); i++)
			{
				if (auto& info = _nodes[i]; info.parent == UINodeIndexInvalid)
				{
					const auto it = info.attributes.find(UIAttributeType::Name);
					if (it != info.attributes.end() && it->second.get_string() == name)
					{
						list.push_back(UINodeIndex(i));
						count++;
					}
				}
			}
		}
		else
		{
			if (const auto info = get_info(parent))
			{
				for (const auto index : info->children)
				{
					const auto child = get_info(index);
					if (!child) continue;

					const auto it = child->attributes.find(UIAttributeType::Name);
					if (it != child->attributes.end() && it->second.get_string() == name)
					{
						list.push_back(index);
						count++;
					}
				}
			}
		}

		return count;
	}

	Size UIDocument::find_nodes_by_name(StringView name,
		List<UINode>& list, UINodeIndex parent)
	{
		List<UINodeIndex> indices;
		const auto count = find_indexes_by_name(name, indices, parent);
		for (const auto& index : indices)
			list.push_back({ *this, index });

		return count;
	}

	Size UIDocument::find_indexes_by_name_recurse(StringView name,
		List<UINodeIndex>& list, UINodeIndex parent) const
	{
		Size count = 0;
		count += find_indexes_by_name(name, list, parent);

		const auto& children = get_node_children(parent);
		for (const auto& child : children)
			count += find_indexes_by_name_recurse(name, list, child);

		return count;
	}

	Size UIDocument::find_nodes_by_name_recurse(StringView name,
		List<UINode>& list, UINodeIndex parent)
	{
		Size count = 0;
		count += find_nodes_by_name(name, list, parent);

		const auto& children = get_node_children(parent);
		for (const auto& child : children)
			count += find_nodes_by_name(name, list, child);

		return count;
	}

	UINodeIndex UIDocument::find_index_by_name(
		StringView name, UINodeIndex parent) const
	{
		if (parent == UINodeIndexInvalid)
		{
			for (unsigned i = 0; i < _nodes.size(); i++)
			{
				if (auto& info = _nodes[i]; info.parent == UINodeIndexInvalid)
				{
					const auto it = info.attributes.find(UIAttributeType::Name);
					if (it != info.attributes.end() && it->second.get_string() == name)
						return UINodeIndex(i);
				}
			}
		}
		else
		{
			if (const auto info = get_info(parent))
			{
				for (const auto index : info->children)
				{
					const auto child = get_info(index);
					if (!child) continue;

					const auto it = child->attributes.find(UIAttributeType::Name);
					if (it != child->attributes.end() && it->second.get_string() == name)
						return index;
				}
			}
		}

		return UINodeIndexInvalid;
	}

	Optional<UINode> UIDocument::find_node_by_name(StringView name, UINodeIndex parent)
	{
		if (const auto index = find_index_by_name(name, parent); index != UINodeIndexInvalid)
			return UINode(*this, index);
		return std::nullopt;
	}

	UINodeIndex UIDocument::find_index_by_name_recurse(StringView name, UINodeIndex parent) const
	{
		if (const auto index = find_index_by_name(name, parent); index != UINodeIndexInvalid)
			return index;

		const auto& children = get_node_children(parent);
		for (const auto& child : children)
		{
			if (auto index = find_index_by_name(name, child); index != UINodeIndexInvalid)
				return index;
		}

		return UINodeIndexInvalid;
	}

	Optional<UINode> UIDocument::find_node_by_name_recurse(StringView name, UINodeIndex parent)
	{
		if (const auto node = find_node_by_name(name, parent); node.has_value())
			return node.value();

		const auto& children = get_node_children(parent);
		for (const auto& child : children)
		{
			if (auto node = find_node_by_name(name, child); node.has_value())
				return node.value();
		}

		return std::nullopt;
	}

	// EVENTS ////////////////////////////////////////////////////////////////////
	void UIDocument::send_event(const UIEvent& evt)
	{
		UIActionType action_type;
		switch (evt.type)
		{
		case UIEventType::Clicked:
			if (const auto action = evt.node.get_action(UIActionType::OnClick); action.has_value())
			{
				if (const auto ptr = std::get_if<UIActionDefault>(&action.value()))
					(*ptr)();
			}
			break;

		case UIEventType::ValueChanged:
			//UC_LOG_DEBUG(_logger) << "Value at " << evt.node.index()
			//	<< " changed to " << std::get<Double>(evt.value);

			if (const auto action = evt.node.get_action(UIActionType::OnChange); action.has_value())
			{
				if (const auto ptr = std::get_if<UIActionValue>(&action.value()))
					(*ptr)(evt.value);
			}
			else if (const auto action = evt.node.get_action(UIActionType::OnChange); action.has_value())
			{
				if (const auto ptr = std::get_if<UIActionDefault>(&action.value()))
					(*ptr)();
			}
			break;
		}
	}

	// RAW INDEX /////////////////////////////////////////////////////////////////
	UINodeIndex UIDocument::create_node(UINodeType type, UINodeIndex parent,
		const UIAttributeDict& attributes, const UIActionDict& actions)
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

		if (parent == UINodeIndexInvalid)
			_roots.push_back(index);

		if (!_event_create_node.empty())
		{
			const UINode node(*this, index);
			_event_create_node.invoke(node);
		}

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
		if (index == UINodeIndexInvalid)
			return _roots;

		static const List<UINodeIndex> s_empty{};
		const auto info = get_info(index);
		return info ? info->children : s_empty;
	}

	Size UIDocument::get_node_children(UINodeIndex index, List<UINodeIndex>& list) const
	{
		if (index == UINodeIndexInvalid)
		{
			list.insert(list.end(), _roots.begin(), _roots.end());
			return _roots.size();
		}

		if (const auto info = get_info(index))
		{
			list.insert(list.end(), info->children.begin(), info->children.end());
			return info->children.size();
		}

		return 0;
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
			// Remove previous value from cache
			if (const auto it = info->attributes.find(UIAttributeType::Uid); it != info->attributes.end())
			{
				String id;
				if (it->second.try_get_string(id))
					_id_dict.erase(id);
			}

			if (value.has_value())
			{
				const auto& variant = value.value();
				info->attributes[type] = variant;

				// Add new value to cache
				String id;
				if (variant.try_get_string(id))
					_id_dict[id] = index;
				else UC_LOG_ERROR(_logger) << "Invalid variant value for name attribute " << variant;
			}
			else
			{
				info->attributes.erase(type);
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

	const UIAttributeDict& UIDocument::get_node_attributes(UINodeIndex index) const
	{
		static const UIAttributeDict s_empty{};
		const auto info = get_info(index);
		return info ? info->attributes : s_empty;
	}

	Size UIDocument::get_node_attributes(UINodeIndex index, UIAttributeDict& dict) const
	{
		if (const auto info = get_info(index))
		{
			for (const auto& [type, value] : info->attributes)
				dict[type] = value;
			return info->attributes.size();
		}

		return 0;
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

	const UIActionDict& UIDocument::get_node_actions(UINodeIndex index) const
	{
		static const UIActionDict s_empty{};
		const auto info = get_info(index);
		return info ? info->actions : s_empty;
	}

	Size UIDocument::get_node_actions(UINodeIndex index, UIActionDict& dict) const
	{
		if (const auto info = get_info(index))
		{
			for (const auto& [type, action] : info->actions)
				dict[type] = action;
			return info->actions.size();
		}

		return 0;
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