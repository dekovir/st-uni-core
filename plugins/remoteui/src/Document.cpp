#include "unicore/remoteui/Document.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore::remoteui
{
	Document::Document(Logger* logger)
		: _logger(logger)
	{
	}

	size_t Document::get_system_memory_use() const
	{
		return
			sizeof(Document) +
			(sizeof(NodeInfo) + sizeof(ElementIndex)) * _node_infos.size();
	}

	Size Document::get_roots(List<Element>& list) const
	{
		return get_node_children(list, Element::Empty);
	}

	List<Element> Document::get_roots() const
	{
		return get_node_children(Element::Empty);
	}

	// FIND //////////////////////////////////////////////////////////////////////
	Element Document::find_by_index(ElementIndex index) const
	{
		const auto info = get_info(index);
		return info != nullptr ? node_from_index(index) : Element::Empty;
	}

	Element Document::find_by_tag(ElementTag tag, const Element& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed find by id. Parent "
					<< parent << " from other document";
				return Element::Empty;
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

		return Element::Empty;
	}

	Size Document::find_all_by_tag(ElementTag tag,
		List<Element>& list, const Element& parent) const
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
			internal_find_all_by_tag(ElementIndex_Invalid, tag, list, count);
		}

		return count;
	}

	Element Document::find_by_name(StringView name, const Element& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed find by name. Parent "
					<< parent << " from other document";
				return Element::Empty;
			}

			const auto info = get_info(parent.index());
			if (const auto it = info->attributes.find(Attribute::Name); it != info->attributes.end())
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

		return Element::Empty;
	}

	Size Document::find_all_by_name(StringView name,
		List<Element>& list, const Element& parent) const
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
			internal_find_all_by_name(ElementIndex_Invalid, name, list, count);
		}

		return count;
	}

	Element Document::query(
		const Predicate<const Element&>& predicate,
		const Element& parent) const
	{
		WriteProtectionGuard guard(_write_protection);
		if (!parent.empty())
		{
			if (parent.document() != this)
			{
				UC_LOG_WARNING(_logger) << "Failed to query. Parent "
					<< parent << " from other document";
				return Element::Empty;
			}

			return internal_query(parent.index(), predicate);
		}

		return internal_query(ElementIndex_Invalid, predicate);
	}

	Size Document::query_all(const Predicate<const Element&>& predicate,
		List<Element>& list, const Element& parent) const
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
			internal_query_all(ElementIndex_Invalid, predicate, list, count);
		}

		return count;
	}

	// EVENTS ////////////////////////////////////////////////////////////////////
	void Document::send_event(const Element node,
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
			info->attributes[Attribute::Value] = value;
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

	void Document::send_event(const UIEvent& evt)
	{
		send_event(evt.node, evt.type, evt.value);
	}

	// RAW INDEX /////////////////////////////////////////////////////////////////
	Element Document::create_node(ElementTag tag,
		const ElementOptions& options, const Element& parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (!parent.empty() && parent.document() != this)
		{
			UC_LOG_ERROR(_logger) << "Failed to create node. Wrong document";
			return Element::Empty;
		}

		if (!parent.empty() && parent.tag() != ElementTag::Group)
		{
			UC_LOG_ERROR(_logger) << "Failed to create node. Only group tag can have children";
			return Element::Empty;
		}

		return node_from_index(internal_create_node(tag, options, parent.index()));
	}

	Element Document::create_group(GroupType type,
		const ElementOptions& options, const Element& parent)
	{
		auto copy_options = options;
		copy_options.attributes[Attribute::Type] = type;
		return create_node(ElementTag::Group, copy_options, parent);
	}

	Element Document::create_visual(VisualType type,
		const ElementOptions& options, const Element& parent)
	{
		auto copy_options = options;
		copy_options.attributes[Attribute::Type] = type;
		return create_node(ElementTag::Visual, copy_options, parent);
	}

	Element Document::create_input(InputType type,
		const ElementOptions& options, const Element& parent)
	{
		auto copy_options = options;
		copy_options.attributes[Attribute::Type] = type;
		return create_node(ElementTag::Input, copy_options, parent);
	}

	Element Document::duplicate(const Element& node, const Element& at_parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		if (!is_node_valid(node))
		{
			UC_LOG_ERROR(_logger) << "Failed to duplicate node. Wrong node";
			return Element::Empty;
		}

		if (!at_parent.empty() && at_parent.tag() != ElementTag::Group)
		{
			UC_LOG_ERROR(_logger) << "Failed to duplicate node. Only group tag can have children";
			return Element::Empty;
		}

		return node_from_index(internal_duplicate(node, at_parent.index()));
	}

	Bool Document::remove_node(const Element& node)
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

	Bool Document::apply_options(const Element& node, const ElementOptions& options)
	{
		if (const auto info = get_info(node))
		{
			// ATTRIBUTES
			for (const auto& key : AttributeKeys)
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
	Bool Document::is_node_valid(const Element& node) const
	{
		return
			node.document() == this &&
			get_info(node.index()) != nullptr;
	}

	Bool Document::get_node_tag(const Element& node, ElementTag& value) const
	{
		if (const auto info = get_info(node))
		{
			value = info->tag;
			return true;
		}

		return false;
	}

	// HIERARCHY /////////////////////////////////////////////////////////////////
	Element Document::get_node_parent(const Element& node) const
	{
		if (const auto info = get_info(node))
			return node_from_index(info->parent);

		return Element::Empty;
	}

	Size Document::get_node_children(List<Element>& list, const Element& node) const
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

	List<Element> Document::get_node_children(const Element& node) const
	{
		List<Element> list;
		get_node_children(list, node);
		return list;
	}

	Element Document::get_node_child(const Element& node, int index) const
	{
		if (!node.empty())
		{
			if (node.document() != this)
			{
				UC_LOG_ERROR(_logger) << "Failed to get node children count. Node "
					<< node << " from other document";
				return Element::Empty;
			}

			if (const auto children = get_children_list(node.index()); children != nullptr)
			{
				if (const auto it = std::find(children->begin(), children->end(), node.index()); it != children->end())
				{
					const auto value = static_cast<ElementIndex::TypeValue>(it - children->begin());
					return node_from_index(ElementIndex(value));
				}
			}
		}

		return Element::Empty;
	}

	Size Document::get_node_children_count(const Element& node) const
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

	int Document::get_node_sibling_index(const Element& node) const
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

	Bool Document::set_node_sibling_index(const Element& node, int new_index)
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

	Element Document::get_node_next_sibling(const Element& node) const
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

		return Element::Empty;
	}

	Element Document::get_node_prev_sibling(const Element& node) const
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

		return Element::Empty;
	}

	// ATTRIBUTES ////////////////////////////////////////////////////////////////
	void Document::set_node_attribute(const Element& node,
		Attribute attribute, const Variant& value)
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

	const Variant& Document::get_node_attribute(const Element& node, Attribute attribute) const
	{
		if (const auto info = get_info(node))
		{
			if (const auto it = info->attributes.find(attribute); it != info->attributes.end())
				return it->second;
		}
		return Variant::Empty;
	}

	Optional<AttributeDict> Document::get_node_attributes(const Element& node) const
	{
		if (const auto info = get_info(node))
			return info->attributes;

		return std::nullopt;
	}

	Bool Document::get_node_attributes(const Element& node, AttributeDict& dict) const
	{
		if (const auto info = get_info(node))
		{
			dict = info->attributes;
			return true;
		}

		return false;
	}

	StringView Document::get_node_name(const Element& node) const
	{
		const auto value = get_node_attribute(node, Attribute::Hidden);
		if (StringView view; value.try_get_string(view))
			return view;

		return {};
	}

	void Document::set_node_name(const Element& node, StringView value)
	{
		set_node_attribute(node, Attribute::Hidden, value);
	}

	Bool Document::get_node_hidden(const Element& node) const
	{
		return get_node_attribute(node, Attribute::Hidden).get_bool();
	}

	void Document::set_node_hidden(const Element& node, Bool value)
	{
		set_node_attribute(node, Attribute::Hidden, value);
	}

	// ACTIONS ///////////////////////////////////////////////////////////////////
	void Document::subscribe_node(const Element& node,
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

	Bool Document::unsubscribe_node(const Element& node, UIActionType type)
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

	Document::NodeInfo* Document::get_info(ElementIndex index)
	{
		const auto it = _node_infos.find(index);
		return it != _node_infos.end() ? &it->second : nullptr;
	}

	const Document::NodeInfo* Document::get_info(ElementIndex index) const
	{
		const auto it = _node_infos.find(index);
		return it != _node_infos.end() ? &it->second : nullptr;
	}

	Document::NodeInfo* Document::get_info(const Element& node)
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	const Document::NodeInfo* Document::get_info(const Element& node) const
	{
		return node.document() == this ? get_info(node.index()) : nullptr;
	}

	UIActionDict* Document::get_actions(ElementIndex index)
	{
		const auto it = _node_actions.find(index);
		return it != _node_actions.end() ? &it->second : nullptr;
	}

	const UIActionDict* Document::get_actions(ElementIndex index) const
	{
		const auto it = _node_actions.find(index);
		return it != _node_actions.end() ? &it->second : nullptr;
	}

	Document::NodeIndexList* Document::get_children_list(ElementIndex index)
	{
		if (index != ElementIndex_Invalid)
		{
			const auto it = _node_children.find(index);
			return it != _node_children.end() ? &it->second : nullptr;
		}

		return !_roots.empty() ? &_roots : nullptr;
	}

	const Document::NodeIndexList* Document::get_children_list(ElementIndex index) const
	{
		if (index != ElementIndex_Invalid)
		{
			const auto it = _node_children.find(index);
			return it != _node_children.end() ? &it->second : nullptr;
		}

		return !_roots.empty() ? &_roots : nullptr;
	}

	ElementIndex Document::create_index()
	{
		return ElementIndex(_last_index++);
	}

	Element Document::node_from_index(ElementIndex index) const
	{
		return { this, index };
	}

	void Document::internal_find_all_by_tag(ElementIndex index,
		ElementTag tag, List<Element>& list, Size& count) const
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

	void Document::internal_find_all_by_name(ElementIndex index,
		StringView name, List<Element>& list, Size& count) const
	{
		const auto info = get_info(index);
		if (const auto it = info->attributes.find(Attribute::Name); it != info->attributes.end())
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

	Element Document::internal_query(ElementIndex index,
		const Predicate<const Element&>& predicate) const
	{
		if (index != ElementIndex_Invalid)
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

		return Element::Empty;
	}

	void Document::internal_query_all(ElementIndex index,
		const Predicate<const Element&>& predicate,
		List<Element>& list, Size& count) const
	{
		if (index != ElementIndex_Invalid)
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

	ElementIndex Document::internal_create_node(ElementTag tag,
		const ElementOptions& options, ElementIndex parent)
	{
		UC_ASSERT_MSG(!_write_protection, "Write protection is On");

		const auto index = create_index();
		auto& info = _node_infos[index];

		info.tag = tag;
		info.parent = ElementIndex_Invalid;
		info.attributes = options.attributes;

		if (!options.actions.empty())
			_node_actions[index] = options.actions;

		if (parent != ElementIndex_Invalid)
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

	ElementIndex Document::internal_duplicate(const Element& node, ElementIndex parent)
	{
		if (const auto info = get_info(node))
		{
			ElementOptions options;
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

		return ElementIndex_Invalid;
	}

	void Document::internal_remove_node(ElementIndex index, Size& count)
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

	bool Document::call_action_default(const UIAction& action, const Element& node)
	{
		if (const auto ptr = std::get_if<ActionDefault>(&action))
		{
			(*ptr)();
			return true;
		}

		return false;
	}

	bool Document::call_action_value(const UIAction& action, const Element& node, const Variant& value)
	{
		if (const auto ptr = std::get_if<UIActionValue>(&action))
		{
			(*ptr)(value);
			return true;
		}

		if (const auto ptr = std::get_if<ActionDefault>(&action))
		{
			(*ptr)();
			return true;
		}

		return false;
	}
}