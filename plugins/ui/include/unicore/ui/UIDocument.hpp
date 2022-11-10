#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/ui/UINode.hpp"
#include "unicore/system/Event.hpp"

namespace unicore
{
	struct UIEvent
	{
		UINode node;
		UIActionType type;
		Variant value;
	};

	class UIDocument : public Resource
	{
		UC_OBJECT_EVENT(create_node, const UINode&);
		UC_OBJECT_EVENT(remove_node, const UINode&);

		UC_OBJECT_EVENT(change_index, const UINode& /* parent */, unsigned /* old */, unsigned /* new */);

		UC_OBJECT_EVENT(set_name, const UINode&, StringView);
		UC_OBJECT_EVENT(set_style, const UINode&, StringView);
		UC_OBJECT_EVENT(set_visible, const UINode&, Bool);
		UC_OBJECT_EVENT(set_attribute, const UINode&, UIAttribute, const Optional<Variant>&);
	public:
		explicit UIDocument(Logger* logger = nullptr);

		UC_NODISCARD size_t get_system_memory_use() const override;

		Size get_roots(List<UINode>& list) const;
		UC_NODISCARD List<UINode> get_roots() const;

		// FIND ////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINode find_by_id(StringView id) const;

		UC_NODISCARD UINode find_by_type(UINodeTag tag,
			const UINode& parent = UINode::Empty) const;
		Size find_all_by_type(UINodeTag tag, List<UINode>& list,
			const UINode& parent = UINode::Empty) const;

		UC_NODISCARD UINode find_by_name(StringView name,
			const UINode& parent = UINode::Empty) const;
		Size find_all_by_name(StringView name, List<UINode>& list,
			const UINode& parent = UINode::Empty) const;

		UC_NODISCARD UINode querry(
			const Predicate<const UINode&>& predicate,
			const UINode& parent = UINode::Empty) const;

		Size querry_all(const Predicate<const UINode&>& predicate,
			List<UINode>& list, const UINode& parent = UINode::Empty) const;

		// EVENTS //////////////////////////////////////////////////////////////////
		void send_event(const UINode node, UIActionType type,
			const Variant& value = Variant::Empty);
		void send_event(const UIEvent& evt);

		// CREATE //////////////////////////////////////////////////////////////////
		UINode create_node(UINodeTag tag, const UINodeOptions& options,
			const UINode& parent = UINode::Empty);

		UINode duplicate(const UINode& node, const UINode& at_parent = UINode::Empty);

		Bool remove_node(const UINode& node);

		// VALUES //////////////////////////////////////////////////////////////////
		UC_NODISCARD Bool is_node_valid(const UINode& node) const;

		UC_NODISCARD Bool get_node_tag(const UINode& node, UINodeTag& value) const;

		UC_NODISCARD Bool get_node_uid(const UINode& node, String& value) const;

		UC_NODISCARD Bool get_node_name(const UINode& node, String& value) const;
		Bool set_node_name(const UINode& node, StringView value);

		UC_NODISCARD Bool get_node_style(const UINode& node, String& value) const;
		Bool set_node_style(const UINode& node, StringView value);

		UC_NODISCARD Bool get_node_visible(const UINode& node, Bool& value) const;
		Bool set_node_visible(const UINode& node, Bool value);

		// HIERARCHY ///////////////////////////////////////////////////////////////
		UC_NODISCARD UINode get_node_parent(const UINode& node) const;

		Size get_node_children(List<UINode>& list, const UINode& node) const;
		UC_NODISCARD List<UINode> get_node_children(const UINode& node) const;
		UC_NODISCARD Size get_node_children_count(const UINode& node) const;

		UC_NODISCARD Optional<unsigned> get_node_sibling_index(const UINode& node) const;
		Bool set_node_sibling_index(const UINode& node, unsigned new_index);

		UC_NODISCARD UINode get_node_next_sibling(const UINode& node) const;
		UC_NODISCARD UINode get_node_prev_sibling(const UINode& node) const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		void set_node_attribute(const UINode& node,
			UIAttribute attribute, const Optional<Variant>& value);

		UC_NODISCARD Optional<Variant> get_node_attribute(
			const UINode& node, UIAttribute attribute) const;

		UC_NODISCARD Optional<UIAttributeDict> get_node_attributes(const UINode& node) const;
		UC_NODISCARD Bool get_node_attributes(const UINode& node, UIAttributeDict& dict) const;

		// ACTIONS /////////////////////////////////////////////////////////////////
		void subscribe_node(const UINode& node, UIActionType type, const UIAction& action);
		Bool unsubscribe_node(const UINode& node, UIActionType type);

	protected:
		struct NodeInfo
		{
			UINodeTag tag = UINodeTag::Group;
			String uid;
			String name;
			String style;
			Bool visible = true;
			UINode::IndexType parent = UINode::InvalidIndex;
			List<UINode::IndexType> children;
			UIAttributeDict attributes;
		};

		Logger* _logger;
		List<UINode::IndexType> _roots;
		Dictionary<UINode::IndexType, NodeInfo> _nodes;
		Dictionary<UINode::IndexType, UIActionDict> _node_actions;
		Dictionary<String, UINode::IndexType> _cached_id;

		mutable bool _write_protection = false;

		struct WriteProtectionGuard
		{
			explicit WriteProtectionGuard(Bool& value)
				: _value(value)
			{
				_value = true;
			}

			~WriteProtectionGuard()
			{
				_value = false;
			}

			Bool& _value;
		};

		UINode::IndexType _last_index = 0;

		NodeInfo* get_info(UINode::IndexType index);
		UC_NODISCARD const NodeInfo* get_info(UINode::IndexType index) const;

		NodeInfo* get_info(const UINode& node);
		UC_NODISCARD const NodeInfo* get_info(const UINode& node) const;

		UIActionDict* get_actions(UINode::IndexType index);
		UC_NODISCARD const UIActionDict* get_actions(UINode::IndexType index) const;

		UINode::IndexType create_index();
		UC_NODISCARD UINode node_from_index(UINode::IndexType index) const;

		void internal_find_all_by_type(UINode::IndexType index,
			UINodeTag tag, List<UINode>& list, Size& count) const;
		void internal_find_all_by_name(UINode::IndexType index,
			StringView name, List<UINode>& list, Size& count) const;

		UINode internal_querry(UINode::IndexType index,
			const Predicate<const UINode&>& predicate) const;

		void internal_querry_all(UINode::IndexType index,
			const Predicate<const UINode&>& predicate,
			List<UINode>& list, Size& count) const;

		UINode internal_duplicate_recurse(const UINode& node, const UINode& parent);
		void internal_remove_node_recurse(UINode::IndexType index, Size& count);

		static bool call_action_default(const UIAction& action, const UINode& node);
		static bool call_action_value(const UIAction& action, const UINode& node, const Variant& value);
	};
}