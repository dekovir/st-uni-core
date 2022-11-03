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
			(sizeof(NodeInfo) + sizeof(UINode::IndexType)) * _nodes.size();
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
	UINode UIDocument::find_by_id(StringView id) const
	{
		const auto key = StringHelper::to_lower(id);
		if (const auto it = _cached_id.find(key); it != _cached_id.end())
			return node_from_index(it->second);

		return UINode::Empty;
	}

	UINode UIDocument::find_by_type(UINodeType type, const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return UINode::Empty;
			}

			const auto& info = get_info(parent.index());
			if (info->type == type)
				return parent;

			for (const auto child_index : info->children)
			{
				const auto child = node_from_index(child_index);
				if (auto find = find_by_type(type, child); find.valid())
					return find;
			}
		}
		else
		{
			for (const auto& root_index : _roots)
			{
				const auto root = node_from_index(root_index);
				if (auto find = find_by_type(type, root); find.valid())
					return find;
			}
		}

		return UINode::Empty;
	}

	Size UIDocument::find_all_by_type(UINodeType type,
		List<UINode>& list, const UINode& parent) const
	{
		Size count = 0;

		WriteProtectionGuard guard(_write_protection);
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return 0;
			}

			internal_find_all_by_type(parent.index(), type, list, count);
		}
		else
		{
			for (const auto& root_index : _roots)
				internal_find_all_by_type(root_index, type, list, count);
		}

		return count;
	}

	UINode UIDocument::find_by_name(StringView name, const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return UINode::Empty;
			}

			const auto& info = get_info(parent.index());
			if (StringHelper::equals(info->name, name))
				return parent;

			for (const auto child_index : info->children)
			{
				const auto child = node_from_index(child_index);
				if (auto find = find_by_name(name, child); find.valid())
					return find;
			}
		}
		else
		{
			for (const auto& root_index : _roots)
			{
				const auto node = node_from_index(root_index);
				if (auto find = find_by_name(name, node); find.valid())
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
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return 0;
			}

			internal_find_all_by_name(parent.index(), name, list, count);
		}
		else
		{
			for (const auto& root_index : _roots)
				internal_find_all_by_name(root_index, name, list, count);
		}

		return count;
	}

	UINode UIDocument::querry(
		const Predicate<const UINode&>& predicate,
		const UINode& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return UINode::Empty;
			}

			return internal_querry(parent.index(), predicate);
		}
		else
		{
			for (const auto& root_index : _roots)
			{
				if (auto find = internal_querry(root_index, predicate); find.valid())
					return find;
			}
		}

		return UINode::Empty;
	}

	Size UIDocument::querry_all(const Predicate<const UINode&>& predicate,
		List<UINode>& list, const UINode& parent) const
	{
		Size count = 0;

		WriteProtectionGuard guard(_write_protection);

		if (parent.valid())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Node from other document";
				return 0;
			}

			internal_querry_all(parent.index(), predicate, list, count);
		}
		else
		{
			for (const auto& root_index : _roots)
				internal_querry_all(root_index, predicate, list, count);
		}

		return count;
	}

	// EVENTS ////////////////////////////////////////////////////////////////////
	void UIDocument::send_event(const UIEvent& evt)
	{
		if (evt.node.document() != this)
			return;

		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		const auto info = get_info(evt.node.index());
		if (!info)
			return;

		switch (evt.type)
		{
		case UIEventType::Clicked:
			UC_LOG_DEBUG(_logger) << "Node " << evt.node << " value has clicked";
			if (const auto action = get_node_action(evt.node, UIActionType::OnClick); action.has_value())
			{
				if (!call_action_default(action.value(), evt.node))
					UC_LOG_WARNING(_logger) << "Failed to call default action";
			}
			break;

		case UIEventType::ValueChanged:
			info->attributes[UIAttributeType::Value] = evt.value;
			UC_LOG_DEBUG(_logger) << "Node " << evt.node << " value changed to " << evt.value;

			if (const auto action = get_node_action(evt.node, UIActionType::OnChange); action.has_value())
			{
				if (!call_action_value(action.value(), evt.node, evt.value))
					UC_LOG_WARNING(_logger) << "Failed to call value action";
			}
			break;

		case UIEventType::MouseEnter:
			//UC_LOG_DEBUG(_logger) << "Mouse enters " << evt.node;
			if (const auto action = get_node_action(evt.node, UIActionType::OnMouseEnter); action.has_value())
			{
				if (!call_action_default(action.value(), evt.node))
					UC_LOG_WARNING(_logger) << "Failed to call default action";
			}
			break;

		case UIEventType::MouseLeave:
			//UC_LOG_DEBUG(_logger) << "Mouse leaves " << evt.node;
			if (const auto action = get_node_action(evt.node, UIActionType::OnMouseLeave); action.has_value())
			{
				if (!call_action_default(action.value(), evt.node))
					UC_LOG_WARNING(_logger) << "Failed to call default action";
			}
			break;
		}
	}

	// RAW INDEX /////////////////////////////////////////////////////////////////
	UINode UIDocument::create_node(UINodeType type,
		const UINodeOptions& options, const UINode& parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (parent.valid() && !is_node_valid(parent))
		{
			// TODO: Implement duplicate from other document
			return UINode::Empty;
		}

		const auto index = create_index();
		auto& info = _nodes[index];

		info.type = type;
		info.uid = StringHelper::to_lower(options.uid);
		info.name = options.name;
		info.visible = options.visible;

		info.parent = UINode::InvalidIndex;
		info.attributes = options.attributes;
		info.actions = options.actions;

		if (parent.valid())
		{
			info.parent = parent.index();

			if (const auto parent_info = get_info(parent))
				parent_info->children.push_back(index);
		}
		else
		{
			_roots.push_back(index);
		}

		if (!info.uid.empty())
		{
			if (_cached_id.find(info.uid) != _cached_id.end())
				UC_LOG_ERROR(_logger) << "Duplicate uid '" << info.uid << "'";

			_cached_id[info.uid] = index;
			UC_LOG_DEBUG(_logger) << "Node at " << index
				<< " registered with '" << info.uid << "' uid";
		}

		const auto node = node_from_index(index);
		_event_create_node.invoke(node);

		return node;
	}

	UINode UIDocument::duplicate(const UINode& node, const UINode& at_parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		return internal_duplicate_recurse(node, at_parent);
	}

	Bool UIDocument::remove_node(const UINode& node)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (!is_node_valid(node))
			return false;

		Size count = 0;
		internal_remove_node_recurse(node.index(), count);
		return count;
	}

	// VALUES ////////////////////////////////////////////////////////////////////
	Bool UIDocument::is_node_valid(const UINode& node) const
	{
		return
			node.document() == this &&
			get_info(node.index()) != nullptr;
	}

	Bool UIDocument::get_node_type(const UINode& node, UINodeType& value) const
	{
		if (const auto info = get_info(node))
		{
			value = info->type;
			return true;
		}

		return false;
	}

	Bool UIDocument::get_node_uid(const UINode& node, String& value) const
	{
		if (const auto info = get_info(node); info && !info->uid.empty())
		{
			value = info->uid;
			return true;
		}

		return false;
	}

	Bool UIDocument::get_node_name(const UINode& node, String& value) const
	{
		if (const auto info = get_info(node); info && !info->name.empty())
		{
			value = info->name;
			return true;
		}

		return false;
	}

	Bool UIDocument::set_node_name(const UINode& node, StringView value)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
		{
			info->name = value;
			_event_set_name.invoke(node, value);
			return true;
		}

		return false;
	}

	Bool UIDocument::get_node_style(const UINode& node, String& value) const
	{
		if (const auto info = get_info(node); info && !info->style.empty())
		{
			value = info->style;
			return true;
		}

		return false;
	}

	Bool UIDocument::set_node_style(const UINode& node, StringView value)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
		{
			info->style = value;
			_event_set_style.invoke(node, value);
			return true;
		}

		return false;
	}

	Bool UIDocument::get_node_visible(const UINode& node, Bool& value) const
	{
		if (const auto info = get_info(node))
		{
			value = info->visible;
			return true;
		}

		return false;
	}

	Bool UIDocument::set_node_visible(const UINode& node, Bool value)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
		{
			if (info->visible != value)
			{
				info->visible = value;
				_event_set_visible.invoke(node, value);
			}
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
		if (node.valid())
		{
			if (const auto& info = get_info(node))
			{
				for (const auto child_index : info->children)
					list.push_back(node_from_index(child_index));

				return info->children.size();
			}

			UC_LOG_WARNING(_logger) << "Invalid node";
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

	Size UIDocument::get_node_children_count(const UINode& node) const
	{
		if (node.valid())
		{
			if (const auto& info = get_info(node))
				return info->children.size();

			UC_LOG_WARNING(_logger) << "Invalid node";
			return 0;
		}

		return _roots.size();
	}

	Optional<unsigned> UIDocument::get_node_sibling_index(const UINode& node) const
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			if (const auto& parent_info = get_info(info->parent))
			{
				auto& children = parent_info->children;
				if (const auto it = std::find(children.begin(), children.end(), node.index()); it != children.end())
					return it - children.begin();
			}
			else
			{
				if (const auto it = std::find(_roots.begin(), _roots.end(), node.index()); it != _roots.end())
					return it - _roots.begin();
			}
		}

		return std::nullopt;
	}

	Bool UIDocument::set_node_sibling_index(const UINode& node, unsigned new_index)
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			if (const auto& parent_info = get_info(info->parent))
			{
				auto& children = parent_info->children;
				if (const auto it = std::find(children.begin(), children.end(), node.index()); it != children.end())
				{
					if (const auto old_index = it - children.begin(); old_index != new_index)
					{
						children.erase(children.begin() + old_index);
						children.insert(children.begin() + new_index, node.index());

						_event_change_index.invoke(node_from_index(info->parent), old_index, new_index);
					}
					return true;
				}
			}
			else
			{
				if (const auto it = std::find(_roots.begin(), _roots.end(), node.index()); it != _roots.end())
				{
					if (const auto old_index = it - _roots.begin(); old_index != new_index)
					{
						_roots.erase(_roots.begin() + old_index);
						_roots.insert(_roots.begin() + new_index, node.index());

						_event_change_index.invoke(UINode(), old_index, new_index);
					}
					return true;
				}
			}
		}

		return false;
	}

	UINode UIDocument::get_node_next_sibling(const UINode& node) const
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			if (const auto& parent_info = get_info(info->parent))
			{
				auto& children = parent_info->children;
				if (const auto it = std::find(children.begin(), children.end(), node.index()); it != children.end())
				{
					if (const auto index = it - children.begin(); index + 1 < children.size())
						return node_from_index(children[index + 1]);
				}
			}
			else
			{
				if (const auto it = std::find(_roots.begin(), _roots.end(), node.index()); it != _roots.end())
				{
					if (const auto index = it - _roots.begin(); index + 1 < _roots.size())
						return node_from_index(_roots[index + 1]);
				}
			}
		}

		return UINode::Empty;
	}

	UINode UIDocument::get_node_prev_sibling(const UINode& node) const
	{
		if (const auto& info = get_info(node); info != nullptr)
		{
			if (const auto& parent_info = get_info(info->parent))
			{
				auto& children = parent_info->children;
				if (const auto it = std::find(children.begin(), children.end(), node.index()); it != children.end())
				{
					if (const auto index = it - children.end(); index > 0)
						return node_from_index(children[index - 1]);
				}
			}
			else
			{
				if (const auto it = std::find(_roots.begin(), _roots.end(), node.index()); it != _roots.end())
				{
					if (const auto index = it - _roots.end(); index > 0)
						return node_from_index(_roots[index - 1]);
				}
			}
		}

		return UINode::Empty;
	}

	// ATTRIBUTES ////////////////////////////////////////////////////////////////
	void UIDocument::set_node_attribute(const UINode& node,
		UIAttributeType type, const Optional<Variant>& value)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
		{
			if (const auto it = info->attributes.find(type); it != info->attributes.end())
			{
				if (value.has_value())
				{
					if (it->second != value.value())
					{
						it->second = value.value();
						if (!_event_set_attribute.empty())
							_event_set_attribute.invoke(node, type, value);
					}
				}
				else
				{
					info->attributes.erase(it);
					if (!_event_set_attribute.empty())
						_event_set_attribute.invoke(node, type, std::nullopt);
				}
			}
			else if (value.has_value())
			{
				info->attributes[type] = value.value();
				if (!_event_set_attribute.empty())
					_event_set_attribute.invoke(node, type, value);
			}
		}
	}

	Optional<Variant> UIDocument::get_node_attribute(
		const UINode& node, UIAttributeType type) const
	{
		if (const auto info = get_info(node))
		{
			if (const auto it = info->attributes.find(type); it != info->attributes.end())
				return it->second;
		}
		return std::nullopt;
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

	// ACTIONS ///////////////////////////////////////////////////////////////////
	void UIDocument::set_node_action(const UINode& node,
		UIActionType type, const Optional<UIAction>& action)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");
		if (const auto info = get_info(node))
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

			_event_set_action.invoke(node, type, action);
		}
	}

	Optional<UIAction> UIDocument::get_node_action(const UINode& node, UIActionType type) const
	{
		if (const auto info = get_info(node))
		{
			if (const auto it = info->actions.find(type); it != info->actions.end())
				return it->second;
		}

		return std::nullopt;
	}

	Optional<UIActionDict> UIDocument::get_node_actions(const UINode& node) const
	{
		if (const auto info = get_info(node))
			return info->actions;

		return std::nullopt;
	}

	Bool UIDocument::get_node_actions(const UINode& node, UIActionDict& dict) const
	{
		if (const auto info = get_info(node))
		{
			dict = info->actions;
			return true;
		}

		return false;
	}

	UIDocument::NodeInfo* UIDocument::get_info(UINode::IndexType index)
	{
		const auto it = _nodes.find(index);
		return it != _nodes.end() ? &it->second : nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(UINode::IndexType index) const
	{
		const auto it = _nodes.find(index);
		return it != _nodes.end() ? &it->second : nullptr;
	}

	UIDocument::NodeInfo* UIDocument::get_info(const UINode& node)
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	const UIDocument::NodeInfo* UIDocument::get_info(const UINode& node) const
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	UINode::IndexType UIDocument::create_index()
	{
		return _last_index++;
	}

	UINode UIDocument::node_from_index(UINode::IndexType index) const
	{
		return { this, index };
	}

	void UIDocument::internal_find_all_by_type(UINode::IndexType index,
		UINodeType type, List<UINode>& list, Size& count) const
	{
		const auto info = get_info(index);
		if (!info) return;

		if (info->type == type)
		{
			list.push_back(node_from_index(index));
			count++;
		}

		for (const auto child_index : info->children)
			internal_find_all_by_type(child_index, type, list, count);
	}

	void UIDocument::internal_find_all_by_name(UINode::IndexType index,
		StringView name, List<UINode>& list, Size& count) const
	{
		const auto info = get_info(index);
		if (!info) return;

		if (StringHelper::equals(info->name, name))
		{
			list.push_back(node_from_index(index));
			count++;
		}

		for (const auto child_index : info->children)
			internal_find_all_by_name(child_index, name, list, count);
	}

	UINode UIDocument::internal_querry(UINode::IndexType index,
		const Predicate<const UINode&>& predicate) const
	{
		if (const auto& info = get_info(index))
		{
			if (const auto node = node_from_index(index); predicate(node))
				return node;

			for (const auto child_index : info->children)
			{
				if (auto find = internal_querry(child_index, predicate); find.valid())
					return find;
			}
		}

		return UINode::Empty;
	}

	void UIDocument::internal_querry_all(UINode::IndexType index,
		const Predicate<const UINode&>& predicate,
		List<UINode>& list, Size& count) const
	{
		const auto info = get_info(index);
		if (!info) return;

		if (const auto node = node_from_index(index); predicate(node))
		{
			list.push_back(node);
			count++;
		}

		for (const auto child_index : info->children)
			internal_querry_all(child_index, predicate, list, count);
	}

	UINode UIDocument::internal_duplicate_recurse(const UINode& node, const UINode& parent)
	{
		if (const auto info = get_info(node))
		{
			UINodeOptions options;
			options.name = info->name;
			options.visible = info->visible;
			options.attributes = info->attributes;
			options.actions = info->actions;

			const auto children = info->children;
			const auto new_node = create_node(info->type, options, parent);

			if (new_node.valid())
			{
				for (const auto& child : children)
					internal_duplicate_recurse(node_from_index(child), new_node);

				return new_node;
			}
		}

		return UINode::Empty;
	}

	void UIDocument::internal_remove_node_recurse(UINode::IndexType index, Size& count)
	{
		const auto it = _nodes.find(index);
		if (it == _nodes.end())
			return;

		const auto& info = it->second;

		for (const auto& child : info.children)
			internal_remove_node_recurse(child, count);

		_event_remove_node.invoke(node_from_index(index));

		_nodes.erase(index);
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