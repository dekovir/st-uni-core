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
		const auto key = StringHelper::to_lower(id);
		const auto it = _cached_id.find(key);
		return it != _cached_id.end() ? it->second : UINodeIndexInvalid;
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
			for (const auto& [index, info] : _nodes)
			{
				if (info.parent == UINodeIndexInvalid)
				{
					if (StringHelper::equals(info.name, name, true))
					{
						list.push_back(index);
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

					if (StringHelper::equals(child->name, name, true))
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
			for (const auto& [index, info] : _nodes)
			{
				if (info.parent == UINodeIndexInvalid)
				{
					if (StringHelper::equals(info.name, name, true))
						return index;
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

					if (StringHelper::equals(child->name, name, true))
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
		if (&evt.node.document() != this)
			return;

		const auto info = get_info(evt.node.index());
		if (!info)
			return;

		switch (evt.type)
		{
		case UIEventType::Clicked:
			UC_LOG_DEBUG(_logger) << "Node " << evt.node << " value has clicked";
			if (const auto action = evt.node.get_action(UIActionType::OnClick); action.has_value())
			{
				if (!call_action_default(action.value(), evt.node))
					UC_LOG_WARNING(_logger) << "Failed to call default action";
			}
			break;

		case UIEventType::ValueChanged:
			info->attributes[UIAttributeType::Value] = evt.value;
			UC_LOG_DEBUG(_logger) << "Node " << evt.node << " value changed to " << evt.value;

			if (const auto action = evt.node.get_action(UIActionType::OnChange); action.has_value())
			{
				if (!call_action_value(action.value(), evt.node, evt.value))
					UC_LOG_WARNING(_logger) << "Failed to call value action";
			}
			break;
		}
	}

	// RAW INDEX /////////////////////////////////////////////////////////////////
	UINodeIndex UIDocument::create_node(UINodeType type,
		UINodeIndex parent, const UINodeOptions& options)
	{
		const auto index = create_index();
		auto& info = _nodes[index];

		info.type = type;
		info.uid = StringHelper::to_lower(options.uid);
		info.name = options.name;
		info.visible = options.visible;

		info.parent = parent;
		info.attributes = options.attributes;
		info.actions = options.actions;

		if (const auto parent_info = get_info(parent))
			parent_info->children.push_back(index);

		if (parent == UINodeIndexInvalid)
			_roots.push_back(index);

		if (!info.uid.empty())
		{
			if (_cached_id.find(info.uid) != _cached_id.end())
				UC_LOG_ERROR(_logger) << "Duplicate uid '" << info.uid << "'";

			_cached_id[info.uid] = index;
			UC_LOG_DEBUG(_logger) << "Node at " << index
				<< " registered with '" << info.uid << "' uid";
		}

		if (!_event_create_node.empty())
		{
			const UINode node(*this, index);
			_event_create_node.invoke(node);
		}

		return index;
	}

	UINodeIndex UIDocument::duplicate_node(UINodeIndex index)
	{
		if (is_node_valid(index))
		{
			const auto parent = get_node_parent(index);
			return internal_duplicate_recurse(index, parent);
		}

		return UINodeIndexInvalid;
	}

	UINodeIndex UIDocument::duplicate_node_at(UINodeIndex index, UINodeIndex parent)
	{
		if (is_node_valid(index))
			return internal_duplicate_recurse(index, parent);

		return UINodeIndexInvalid;
	}

	Size UIDocument::remove_node(UINodeIndex index)
	{
		if (index == UINodeIndexInvalid)
			return 0;

		Size count = 0;
		internal_remove_node_recurse(index, count);
		return count;
	}

	// VALUES ////////////////////////////////////////////////////////////////////
	Bool UIDocument::is_node_valid(UINodeIndex index) const
	{
		return get_info(index) != nullptr;
	}

	const String& UIDocument::get_node_uid(UINodeIndex index) const
	{
		static const String s_empty;
		const auto info = get_info(index);
		return info ? info->uid : s_empty;
	}

	const String& UIDocument::get_node_name(UINodeIndex index) const
	{
		static const String s_empty;
		const auto info = get_info(index);
		return info ? info->name : s_empty;
	}

	Bool UIDocument::set_node_name(UINodeIndex index, StringView name)
	{
		static const String s_empty;
		if (const auto info = get_info(index))
		{
			info->name = name;
			if (!_event_set_name.empty())
			{
				const auto node = node_from_index(index);
				_event_set_name.invoke(node, name);
			}
			return true;
		}

		return false;
	}

	Bool UIDocument::get_node_visible(UINodeIndex index) const
	{
		const auto info = get_info(index);
		return info ? info->visible : false;
	}

	Bool UIDocument::set_node_visible(UINodeIndex index, Bool value)
	{
		if (const auto info = get_info(index))
		{
			if (info->visible != value)
			{
				info->visible = value;
				if (!_event_set_visible.empty())
				{
					const auto node = node_from_index(index);
					_event_set_visible.invoke(node, value);
				}
			}
			return true;
		}

		return false;
	}

	Optional<UINodeType> UIDocument::get_node_type(UINodeIndex index) const
	{
		if (const auto info = get_info(index))
			return info->type;
		return std::nullopt;
	}

	// HIERARCHY /////////////////////////////////////////////////////////////////
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

	// ATTRIBUTES ////////////////////////////////////////////////////////////////
	void UIDocument::set_node_attribute(UINodeIndex index,
		UIAttributeType type, const Optional<Variant>& value)
	{
		if (const auto info = get_info(index))
		{
			if (const auto it = info->attributes.find(type); it != info->attributes.end())
			{
				if (value.has_value())
				{
					if (it->second != value.value())
					{
						it->second = value.value();
						if (!_event_set_attribute.empty())
							_event_set_attribute.invoke(node_from_index(index), type, value);
					}
				}
				else
				{
					info->attributes.erase(it);
					if (!_event_set_attribute.empty())
						_event_set_attribute.invoke(node_from_index(index), type, std::nullopt);
				}
			}
			else if (value.has_value())
			{
				info->attributes[type] = value.value();
				if (!_event_set_attribute.empty())
					_event_set_attribute.invoke(node_from_index(index), type, value);
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
		const auto it = _nodes.find(index);
		return it != _nodes.end() ? &it->second : nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(UINodeIndex index) const
	{
		const auto it = _nodes.find(index);
		return it != _nodes.end() ? &it->second : nullptr;
	}

	UINodeIndex UIDocument::create_index()
	{
		const auto index = _last_index;
		_last_index.value++;
		return index;
	}

	UINode UIDocument::node_from_index(UINodeIndex index)
	{
		return { *this, index };
	}

	UINodeIndex UIDocument::internal_duplicate_recurse(UINodeIndex index, UINodeIndex parent)
	{
		if (const auto info = get_info(index))
		{
			UINodeOptions options;
			options.name = info->name;
			options.visible = info->visible;
			options.attributes = info->attributes;
			options.actions = info->actions;

			const auto children = info->children;
			const auto new_parent = create_node(info->type, parent, options);

			for (const auto& child : children)
				internal_duplicate_recurse(child, new_parent);

			return new_parent;
		}

		return UINodeIndexInvalid;
	}

	void UIDocument::internal_remove_node_recurse(UINodeIndex index, Size& count)
	{
		const auto it = _nodes.find(index);
		if (it == _nodes.end())
			return;

		auto& info = it->second;

		for (const auto& child : info.children)
			internal_remove_node_recurse(child, count);

		_nodes.erase(index);

		if (!_event_remove_node.empty())
			_event_remove_node.invoke(index);
	}

	bool UIDocument::call_action_default(const UIAction& action, const UINode& node)
	{
		if (const auto ptr = std::get_if<UIActionDefault>(&action))
		{
			(*ptr)();
			return true;
		}
		if (const auto ptr = std::get_if<UIActionNodeDefault>(&action))
		{
			(*ptr)(node);
			return true;
		}

		return false;
	}

	bool UIDocument::call_action_value(const UIAction& action, const UINode& node, const Variant& value)
	{
		if (const auto ptr = std::get_if<UIActionValue>(&action))
		{
			(*ptr)(value);
			return true;
		}
		if (const auto ptr = std::get_if<UIActionNodeValue>(&action))
		{
			(*ptr)(node, value);
			return true;
		}

		return false;
	}
}