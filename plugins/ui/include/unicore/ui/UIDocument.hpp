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
		UC_OBJECT_EVENT(reorder_children, const UINode&);

		UC_OBJECT_EVENT(set_name, const UINode&, StringView);
		UC_OBJECT_EVENT(set_style, const UINode&, StringView);
		UC_OBJECT_EVENT(set_hidden, const UINode&, Bool);
		UC_OBJECT_EVENT(set_attribute, const UINode&, UIAttribute, const Variant&);
	public:
		explicit UIDocument(Logger* logger = nullptr);

		UC_NODISCARD size_t get_system_memory_use() const override;

		Size get_roots(List<UINode>& list) const;
		UC_NODISCARD List<UINode> get_roots() const;

		// FIND ////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINode find_by_id(StringView id) const;
		UC_NODISCARD UINode find_by_index(UINodeIndex index) const;

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

		UINode create_group(UIGroupType type, const UINodeOptions& options,
			const UINode& parent = UINode::Empty);
		UINode create_visual(UIVisualType type, const UINodeOptions& options,
			const UINode& parent = UINode::Empty);
		UINode create_input(UIInputType type, const UINodeOptions& options,
			const UINode& parent = UINode::Empty);

		UINode duplicate(const UINode& node, const UINode& at_parent = UINode::Empty);

		Bool remove_node(const UINode& node);

		Bool apply_options(const UINode& node, const UINodeOptions& options);

		// VALUES //////////////////////////////////////////////////////////////////
		UC_NODISCARD Bool is_node_valid(const UINode& node) const;

		UC_NODISCARD Bool get_node_tag(const UINode& node, UINodeTag& value) const;

		UC_NODISCARD Bool get_node_uid(const UINode& node, String& value) const;

		UC_NODISCARD Bool get_node_name(const UINode& node, String& value) const;
		Bool set_node_name(const UINode& node, StringView value);

		// HIERARCHY ///////////////////////////////////////////////////////////////
		UC_NODISCARD UINode get_node_parent(const UINode& node) const;

		Size get_node_children(List<UINode>& list, const UINode& node) const;
		UC_NODISCARD List<UINode> get_node_children(const UINode& node) const;
		UC_NODISCARD Size get_node_children_count(const UINode& node) const;

		UC_NODISCARD UINode get_node_child(const UINode& node, Size index) const;

		UC_NODISCARD Optional<unsigned> get_node_sibling_index(const UINode& node) const;
		Bool set_node_sibling_index(const UINode& node, unsigned new_index);

		UC_NODISCARD UINode get_node_next_sibling(const UINode& node) const;
		UC_NODISCARD UINode get_node_prev_sibling(const UINode& node) const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		void set_node_attribute(const UINode& node,
			UIAttribute attribute, const Variant& value);

		UC_NODISCARD const Variant& get_node_attribute(const UINode& node, UIAttribute attribute) const;

		UC_NODISCARD Optional<UIAttributeDict> get_node_attributes(const UINode& node) const;
		UC_NODISCARD Bool get_node_attributes(const UINode& node, UIAttributeDict& dict) const;

		void set_node_hidden(const UINode& node, Bool value) { set_node_attribute(node, UIAttribute::Hidden, value); }

		// ACTIONS /////////////////////////////////////////////////////////////////
		void subscribe_node(const UINode& node, UIActionType type, const UIAction& action);
		Bool unsubscribe_node(const UINode& node, UIActionType type);

	protected:
		struct NodeInfo
		{
			UINodeTag tag = UINodeTag::Group;
			String uid;
			String name;
			// UInt16 sibling_index; ? Optimization
			UINodeIndex parent = UINodeIndex_Invalid;
			UIAttributeDict attributes;
		};

		using NodeIndexList = List<UINodeIndex>;

		Logger* _logger;
		NodeIndexList _roots;

		// TODO: Optimize with list
		Dictionary<UINodeIndex, NodeInfo> _node_infos;
		Dictionary<UINodeIndex, UIActionDict> _node_actions;
		Dictionary<UINodeIndex, NodeIndexList> _node_children;
		Dictionary<String, UINodeIndex> _cached_id;

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

		UINodeIndex::TypeValue _last_index = 0;

		NodeInfo* get_info(UINodeIndex index);
		UC_NODISCARD const NodeInfo* get_info(UINodeIndex index) const;

		NodeInfo* get_info(const UINode& node);
		UC_NODISCARD const NodeInfo* get_info(const UINode& node) const;

		UIActionDict* get_actions(UINodeIndex index);
		UC_NODISCARD const UIActionDict* get_actions(UINodeIndex index) const;

		NodeIndexList* get_children_index(UINodeIndex index);
		UC_NODISCARD const NodeIndexList* get_children_index(UINodeIndex index) const;

		UINodeIndex create_index();
		UC_NODISCARD UINode node_from_index(UINodeIndex index) const;

		void internal_find_all_by_type(UINodeIndex index,
			UINodeTag tag, List<UINode>& list, Size& count) const;
		void internal_find_all_by_name(UINodeIndex index,
			StringView name, List<UINode>& list, Size& count) const;

		UINode internal_querry(UINodeIndex index,
			const Predicate<const UINode&>& predicate) const;

		void internal_querry_all(UINodeIndex index,
			const Predicate<const UINode&>& predicate,
			List<UINode>& list, Size& count) const;

		UINodeIndex internal_create_node(UINodeTag tag,
			const UINodeOptions& options, UINodeIndex parent);
		UINodeIndex internal_duplicate(const UINode& node, UINodeIndex parent);
		void internal_remove_node(UINodeIndex index, Size& count);

		static bool call_action_default(const UIAction& action, const UINode& node);
		static bool call_action_value(const UIAction& action, const UINode& node, const Variant& value);
	};
}