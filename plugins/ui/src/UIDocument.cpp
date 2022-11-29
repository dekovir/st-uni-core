#include "unicore/ui/UIDocument.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	UIDocument::UIDocument(Logger* logger)
		: _logger(logger)
	{
	}

	size_t UIDocument::get_system_memory_use() const
	{
		return
			sizeof(UIDocument) +
			(sizeof(NodeInfo) + sizeof(UINodeIndex)) * _node_infos.size();
	}

	Size UIDocument::get_roots(List<UINode>& list) const
	{
		return get_node_children(list, UINode::Empty);
	}

	List<UINode> UIDocument::get_roots() const
	{
		return get_node_children(UINode::Empty);
	}

	// FIND //////////////////////////////////////////////////////////////////////
	UINode UIDocument::find_by_index(UINodeIndex index) const
	{
		const auto info = get_info(index);
		return info != nullptr ? node_from_index(index) : UINode::Empty;
	}

	UINode UIDocument::find_by_tag(UINodeTag tag, const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed find by id. Parent "
					<< parent << " from other document";
				return UINode::Empty;
			}

			const auto& info = get_info(parent.index());
			if (info->tag == tag)
				return parent;
		}

		if (const auto children = get_children_list(parent.index()))
		{
			for (const auto child_index : *children)
			{
				const auto child = node_from_index(child_index);
				if (auto find = find_by_tag(tag, child); !find.empty())
					return find;
			}
		}

		return UINode::Empty;
	}

	Size UIDocument::find_all_by_tag(UINodeTag tag,
		List<UINode>& list, const UINode& parent) const
	{
		Size count = 0;

		WriteProtectionGuard guard(_write_protection);
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed to find all by type. Parent "
					<< parent << " from other document";
				return 0;
			}

			internal_find_all_by_tag(parent.index(), tag, list, count);
		}
		else
		{
			internal_find_all_by_tag(UINodeIndex_Invalid, tag, list, count);
		}

		return count;
	}

	UINode UIDocument::find_by_name(StringView name, const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed find by name. Parent "
					<< parent << " from other document";
				return UINode::Empty;
			}

			const auto info = get_info(parent.index());
			if (const auto it = info->attributes.find(UIAttribute::Name); it != info->attributes.end())
			{
				StringView info_name;
				if (it->second.try_get_string(info_name) &&
					StringHelper::equals(info_name, name))
					return parent;
			}
		}

		if (const auto children = get_children_list(parent.index()))
		{
			for (const auto child_index : *children)
			{
				const auto child = node_from_index(child_index);
				if (auto find = find_by_name(name, child); !find.empty())
					return find;
			}
		}

		return UINode::Empty;
	}

	Size UIDocument::find_all_by_name(StringView name,
		List<UINode>& list, const UINode& parent) const
	{
		Size count = 0;

		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed find all by names. Parent "
					<< parent << " from other document";
				return 0;
			}

			internal_find_all_by_name(parent.index(), name, list, count);
		}
		else
		{
			internal_find_all_by_name(UINodeIndex_Invalid, name, list, count);
		}

		return count;
	}

	UINode UIDocument::query(
		const Predicate<const UINode&>& predicate,
		const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed to query. Parent "
					<< parent << " from other document";
				return UINode::Empty;
			}

			return internal_query(parent.index(), predicate);
		}

		return internal_query(UINodeIndex_Invalid, predicate);
	}

	Size UIDocument::query_all(const Predicate<const UINode&>& predicate,
		List<UINode>& list, const UINode& parent) const
	{
		Size count = 0;

		WriteProtectionGuard guard(_write_protection);

		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed query all. Parent "
					<< parent << " from other document";
				return 0;
			}

			internal_query_all(parent.index(), predicate, list, count);
		}
		else
		{
			internal_query_all(UINodeIndex_Invalid, predicate, list, count);
		}

		return count;
	}

	// EVENTS ////////////////////////////////////////////////////////////////////
	void UIDocument::send_event(const UINode node,
		UIActionType type, const Variant& value)
	{
		if (node.empty())
		{
			UC_LOG_WARNING(_logger) << "Can't send event - Node " << node << " is empty";
			return;
		}

		if (node.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Can't send event - Node " << node << " from other document";
			return;
		}

		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		const auto info = get_info(node.index());
		if (!info)
			return;

		const auto actions = get_actions(node.index());

		switch (type)
		{
		case UIActionType::OnClick:
			UC_LOG_DEBUG(_logger) << "Node " << node << " value has clicked";
		case UIActionType::OnMouseEnter:
		case UIActionType::OnMouseLeave:
			if (actions != nullptr)
			{
				if (const auto it = actions->find(type); it != actions->end())
				{
					if (!call_action_default(it->second, node))
						UC_LOG_WARNING(_logger) << "Failed to call default action";
				}
			}
			break;

		case UIActionType::OnChange:
			info->attributes[UIAttribute::Value] = value;
			UC_LOG_DEBUG(_logger) << "Node " << node << " value changed to " << value;
			if (actions != nullptr)
			{
				if (const auto it = actions->find(UIActionType::OnChange); it != actions->end())
				{
					if (!call_action_value(it->second, node, value))
						UC_LOG_WARNING(_logger) << "Failed to call value action";
				}
			}
			break;
		}
	}

	void UIDocument::send_event(const UIEvent& evt)
	{
		send_event(evt.node, evt.type, evt.value);
	}

	// RAW INDEX /////////////////////////////////////////////////////////////////
	UINode UIDocument::create_node(UINodeTag tag,
		const UINodeOptions& options, const UINode& parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (!parent.empty() && parent.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to create node. Wrong document";
			return UINode::Empty;
		}

		if (!parent.empty() && parent.tag() != UINodeTag::Group)
		{
			UC_LOG_ERROR(_logger) << "Failed to create node. Only group tag can have children";
			return UINode::Empty;
		}

		return node_from_index(internal_create_node(tag, options, parent.index()));
	}

	UINode UIDocument::create_group(UIGroupType type,
		const UINodeOptions& options, const UINode& parent)
	{
		auto copy_options = options;
		copy_options.attributes[UIAttribute::Type] = type;
		return create_node(UINodeTag::Group, copy_options, parent);
	}

	UINode UIDocument::create_visual(UIVisualType type,
		const UINodeOptions& options, const UINode& parent)
	{
		auto copy_options = options;
		copy_options.attributes[UIAttribute::Type] = type;
		return create_node(UINodeTag::Visual, copy_options, parent);
	}

	UINode UIDocument::create_input(UIInputType type,
		const UINodeOptions& options, const UINode& parent)
	{
		auto copy_options = options;
		copy_options.attributes[UIAttribute::Type] = type;
		return create_node(UINodeTag::Input, copy_options, parent);
	}

	UINode UIDocument::duplicate(const UINode& node, const UINode& at_parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (!is_node_valid(node))
		{
			UC_LOG_ERROR(_logger) << "Failed to duplicate node. Wrong node";
			return UINode::Empty;
		}

		if (!at_parent.empty() && at_parent.tag() != UINodeTag::Group)
		{
			UC_LOG_ERROR(_logger) << "Failed to duplicate node. Only group tag can have children";
			return UINode::Empty;
		}

		return node_from_index(internal_duplicate(node, at_parent.index()));
	}

	Bool UIDocument::remove_node(const UINode& node)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (!is_node_valid(node))
		{
			UC_LOG_WARNING(_logger) << "Failed to remove node. " << node << " is invalid";
			return false;
		}

		Size count = 0;
		internal_remove_node(node.index(), count);
		return count > 0;
	}

	Bool UIDocument::apply_options(const UINode& node, const UINodeOptions& options)
	{
		if (const auto info = get_info(node))
		{
			// ATTRIBUTES
			for (const auto& key : UIAttributeKeys)
			{
				auto it = info->attributes.find(key);
				auto jt = options.attributes.find(key);

				if (it != info->attributes.end() &&
					jt != options.attributes.end())
				{
					if (it->second != jt->second)
					{
						info->attributes[key] = jt->second;
						_event_set_attribute(node, key, jt->second);
					}
				}
				else if (it != info->attributes.end())
				{
					info->attributes.erase(it);
					_event_set_attribute(node, key, Variant::Empty);
				}
				else if (jt != options.attributes.end())
				{
					info->attributes[key] = jt->second;
					_event_set_attribute(node, key, jt->second);
				}
			}

			// ACTIONS
			const auto it = _node_actions.find(node.index());
			if (it != _node_actions.end())
			{
				if (options.actions.empty())
					_node_actions.erase(it);
				else it->second = options.actions;
			}
			else if (!options.actions.empty())
			{
				_node_actions[node.index()] = options.actions;
			}

			return true;
		}

		return false;
	}

	// VALUES ////////////////////////////////////////////////////////////////////
	Bool UIDocument::is_node_valid(const UINode& node) const
	{
		return
			node.document() == this &&
			get_info(node.index()) != nullptr;
	}

	Bool UIDocument::get_node_tag(const UINode& node, UINodeTag& value) const
	{
		if (const auto info = get_info(node))
		{
			value = info->tag;
			return true;
		}

		return false;
	}

	// HIERARCHY /////////////////////////////////////////////////////////////////
	UINode UIDocument::get_node_parent(const UINode& node) const
	{
		if (const auto info = get_info(node))
			return node_from_index(info->parent);

		return UINode::Empty;
	}

	Size UIDocument::get_node_children(List<UINode>& list, const UINode& node) const
	{
		if (!node.empty())
		{
			if (node.document() != this)
			{
				UC_LOG_ERROR(_logger) << "Failed to get children. Node "
					<< node << " from other document";
				return 0;
			}

			if (const auto children = get_children_list(node.index()))
			{
				for (const auto child_index : *children)
					list.push_back(node_from_index(child_index));
				return children->size();
			}

			return 0;
		}

		for (const auto root_index : _roots)
			list.push_back(node_from_index(root_index));

		return _roots.size();
	}

	List<UINode> UIDocument::get_node_children(const UINode& node) const
	{
		List<UINode> list;
		get_node_children(list, node);
		return list;
	}

	UINode UIDocument::get_node_child(const UINode& node, int index) const
	{
		if (!node.empty())
		{
			if (node.document() != this)
			{
				UC_LOG_ERROR(_logger) << "Failed to get node children count. Node "
					<< node << " from other document";
				return UINode::Empty;
			}

			if (const auto children = get_children_list(node.index()); children != nullptr)
			{
				if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
				{
					const auto value = static_cast<UINodeIndex::TypeValue>(it - children->begin());
					return node_from_index(UINodeIndex(value));
				}
			}
		}

		return UINode::Empty;
	}

	Size UIDocument::get_node_children_count(const UINode& node) const
	{
		if (!node.empty())
		{
			if (node.document() != this)
			{
				UC_LOG_ERROR(_logger) << "Failed to get node children count. Node "
					<< node << " from other document";
				return 0;
			}

			if (const auto children = get_children_list(node.index()))
				return children->size();

			return 0;
		}

		return _roots.size();
	}

	int UIDocument::get_node_sibling_index(const UINode& node) const
	{
		if (!node.empty() && node.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to set node "
				<< node << " sibling index. Wrong document";
			return -1;
		}

		if (const auto& info = get_info(node); info != nullptr)
		{
			const auto children = get_children_list(info->parent);
			UC_ASSERT_MSG(children != nullptr, "Children is null");

			if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
				return it - children->begin();

			UC_ASSERT_ALWAYS_MSG("Index not found");
		}

		return -1;
	}

	Bool UIDocument::set_node_sibling_index(const UINode& node, int new_index)
	{
		if (!node.empty() && node.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to set node "
				<< node << " sibling index. Wrong document";
			return false;
		}

		if (const auto& info = get_info(node.index()); info != nullptr)
		{
			if (const auto children = get_children_list(info->parent); children != nullptr)
			{
				new_index = Math::clamp<int>(new_index, 0, children->size() - 1);

				if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
				{
					if (const auto old_index = it - children->begin(); old_index != new_index)
					{
						children->erase(children->begin() + old_index);
						children->insert(children->begin() + new_index, node.index());

						_event_reorder_children.invoke(node_from_index(info->parent));
					}
					return true;
				}

				UC_ASSERT_ALWAYS_MSG("Index not found");
			}
		}

		return false;
	}

	UINode UIDocument::get_node_next_sibling(const UINode& node) const
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			const auto children = get_children_list(info->parent);
			UC_ASSERT_MSG(children != nullptr, "Children is null");

			if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
			{
				if (const auto index = it - children->begin(); index + 1 < children->size())
					return node_from_index(children->at(index + 1));
			}

			UC_ASSERT_ALWAYS_MSG("Index not found");
		}

		return UINode::Empty;
	}

	UINode UIDocument::get_node_prev_sibling(const UINode& node) const
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			const auto children = get_children_list(info->parent);
			UC_ASSERT_MSG(children != nullptr, "Children is null");

			if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
			{
				if (const auto index = it - children->end(); index > 0)
					return node_from_index(children->at(index - 1));
			}

			UC_ASSERT_ALWAYS_MSG("Index not found");
		}

		return UINode::Empty;
	}

	// ATTRIBUTES ////////////////////////////////////////////////////////////////
	void UIDocument::set_node_attribute(const UINode& node,
		UIAttribute attribute, const Variant& value)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
		{
			if (const auto it = info->attributes.find(attribute); it != info->attributes.end())
			{
				if (value != Variant::Empty)
				{
					if (it->second != value)
					{
						it->second = value;
						if (!_event_set_attribute.empty())
							_event_set_attribute.invoke(node, attribute, value);
					}
				}
				else
				{
					info->attributes.erase(it);
					if (!_event_set_attribute.empty())
						_event_set_attribute.invoke(node, attribute, Variant::Empty);
				}
			}
			else if (value != Variant::Empty)
			{
				info->attributes[attribute] = value;
				if (!_event_set_attribute.empty())
					_event_set_attribute.invoke(node, attribute, value);
			}
		}
	}

	const Variant& UIDocument::get_node_attribute(const UINode& node, UIAttribute attribute) const
	{
		if (const auto info = get_info(node))
		{
			if (const auto it = info->attributes.find(attribute); it != info->attributes.end())
				return it->second;
		}
		return Variant::Empty;
	}

	Optional<UIAttributeDict> UIDocument::get_node_attributes(const UINode& node) const
	{
		if (const auto info = get_info(node))
			return info->attributes;

		return std::nullopt;
	}

	Bool UIDocument::get_node_attributes(const UINode& node, UIAttributeDict& dict) const
	{
		if (const auto info = get_info(node))
		{
			dict = info->attributes;
			return true;
		}

		return false;
	}

	StringView UIDocument::get_node_name(const UINode& node) const
	{
		const auto value = get_node_attribute(node, UIAttribute::Hidden);
		if (StringView view; value.try_get_string(view))
			return view;

		return {};
	}

	void UIDocument::set_node_name(const UINode& node, StringView value)
	{
		set_node_attribute(node, UIAttribute::Hidden, value);
	}

	Bool UIDocument::get_node_hidden(const UINode& node) const
	{
		return get_node_attribute(node, UIAttribute::Hidden).get_bool();
	}

	void UIDocument::set_node_hidden(const UINode& node, Bool value)
	{
		set_node_attribute(node, UIAttribute::Hidden, value);
	}

	// ACTIONS ///////////////////////////////////////////////////////////////////
	void UIDocument::subscribe_node(const UINode& node,
		UIActionType type, const UIAction& action)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (node.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to subscribe. Node "
				<< node << " from other document";
			return;
		}

		_node_actions[node.index()][type] = action;
	}

	Bool UIDocument::unsubscribe_node(const UINode& node, UIActionType type)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (node.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to unsubscribe. Node "
				<< node << " from other document";
			return false;
		}

		if (const auto actions = get_actions(node.index()); actions != nullptr)
		{
			if (const auto it = actions->find(type); it != actions->end())
			{
				actions->erase(it);
				return true;
			}
		}

		return false;
	}

	UIDocument::NodeInfo* UIDocument::get_info(UINodeIndex index)
	{
		const auto it = _node_infos.find(index);
		return it != _node_infos.end() ? &it->second : nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(UINodeIndex index) const
	{
		const auto it = _node_infos.find(index);
		return it != _node_infos.end() ? &it->second : nullptr;
	}

	UIDocument::NodeInfo* UIDocument::get_info(const UINode& node)
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(const UINode& node) const
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	UIActionDict* UIDocument::get_actions(UINodeIndex index)
	{
		const auto it = _node_actions.find(index);
		return it != _node_actions.end() ? &it->second : nullptr;
	}

	const UIActionDict* UIDocument::get_actions(UINodeIndex index) const
	{
		const auto it = _node_actions.find(index);
		return it != _node_actions.end() ? &it->second : nullptr;
	}

	UIDocument::NodeIndexList* UIDocument::get_children_list(UINodeIndex index)
	{
		if (index != UINodeIndex_Invalid)
		{
			const auto it = _node_children.find(index);
			return it != _node_children.end() ? &it->second : nullptr;
		}

		return !_roots.empty() ? &_roots : nullptr;
	}

	const UIDocument::NodeIndexList* UIDocument::get_children_list(UINodeIndex index) const
	{
		if (index != UINodeIndex_Invalid)
		{
			const auto it = _node_children.find(index);
			return it != _node_children.end() ? &it->second : nullptr;
		}

		return !_roots.empty() ? &_roots : nullptr;
	}

	UINodeIndex UIDocument::create_index()
	{
		return UINodeIndex(_last_index++);
	}

	UINode UIDocument::node_from_index(UINodeIndex index) const
	{
		return { this, index };
	}

	void UIDocument::internal_find_all_by_tag(UINodeIndex index,
		UINodeTag tag, List<UINode>& list, Size& count) const
	{
		if (const auto info = get_info(index); info && info->tag == tag)
		{
			list.push_back(node_from_index(index));
			count++;
		}

		if (const auto children = get_children_list(index); children != nullptr)
		{
			for (const auto child_index : *children)
				internal_find_all_by_tag(child_index, tag, list, count);
		}
	}

	void UIDocument::internal_find_all_by_name(UINodeIndex index,
		StringView name, List<UINode>& list, Size& count) const
	{
		const auto info = get_info(index);
		if (const auto it = info->attributes.find(UIAttribute::Name); it != info->attributes.end())
		{
			StringView info_name;
			if (it->second.try_get_string(info_name) &&
				StringHelper::equals(info_name, name))
			{
				list.push_back(node_from_index(index));
				count++;
			}
		}

		if (const auto children = get_children_list(index); children != nullptr)
		{
			for (const auto child_index : *children)
				internal_find_all_by_name(child_index, name, list, count);
		}
	}

	UINode UIDocument::internal_query(UINodeIndex index,
		const Predicate<const UINode&>& predicate) const
	{
		if (index != UINodeIndex_Invalid)
		{
			if (const auto node = node_from_index(index); predicate(node))
				return node;
		}

		if (const auto children = get_children_list(index); children != nullptr)
		{
			for (const auto child_index : *children)
			{
				if (auto find = internal_query(child_index, predicate); !find.empty())
					return find;
			}
		}

		return UINode::Empty;
	}

	void UIDocument::internal_query_all(UINodeIndex index,
		const Predicate<const UINode&>& predicate,
		List<UINode>& list, Size& count) const
	{
		if (index != UINodeIndex_Invalid)
		{
			if (const auto node = node_from_index(index); predicate(node))
			{
				list.push_back(node);
				count++;
			}
		}

		if (const auto children = get_children_list(index); children != nullptr)
		{
			for (const auto child_index : *children)
				internal_query_all(child_index, predicate, list, count);
		}
	}

	UINodeIndex UIDocument::internal_create_node(UINodeTag tag,
		const UINodeOptions& options, UINodeIndex parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		const auto index = create_index();
		auto& info = _node_infos[index];

		info.tag = tag;
		info.parent = UINodeIndex_Invalid;
		info.attributes = options.attributes;

		if (!options.actions.empty())
			_node_actions[index] = options.actions;

		if (parent != UINodeIndex_Invalid)
		{
			info.parent = parent;
			_node_children[parent].push_back(index);
		}
		else
		{
			_roots.push_back(index);
		}

		if (!_event_create_node.empty())
			_event_create_node.invoke(node_from_index(index));

		return index;
	}

	UINodeIndex UIDocument::internal_duplicate(const UINode& node, UINodeIndex parent)
	{
		if (const auto info = get_info(node))
		{
			UINodeOptions options;
			options.attributes = info->attributes;

			if (const auto actions = get_actions(node.index()); actions != nullptr)
				options.actions = *actions;

			const auto new_index = internal_create_node(info->tag, options, parent);
			if (const auto children = get_children_list(node.index()); children != nullptr)
			{
				for (const auto& child : *children)
					internal_duplicate(node_from_index(child), new_index);
			}

			return new_index;
		}

		return UINodeIndex_Invalid;
	}

	void UIDocument::internal_remove_node(UINodeIndex index, Size& count)
	{
		// REMOVE CHILDREN
		if (const auto it = _node_children.find(index); it != _node_children.end())
		{
			for (const auto child_index : it->second)
				internal_remove_node(child_index, count);

			_node_children.erase(it);
		}

		// REMOVE ACTIONS
		if (const auto it = _node_actions.find(index); it != _node_actions.end())
			_node_actions.erase(it);

		if (const auto it = _node_infos.find(index); it != _node_infos.end())
		{
			if (!_event_remove_node.empty())
				_event_remove_node.invoke(node_from_index(index));

			_node_infos.erase(index);
		}
	}

	bool UIDocument::call_action_default(const UIAction& action, const UINode& node)
	{
		if (const auto ptr = std::get_if<UIActionDefault>(&action))
		{
			(*ptr)();
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

		if (const auto ptr = std::get_if<UIActionDefault>(&action))
		{
			(*ptr)();
			return true;
		}

		return false;
	}
}