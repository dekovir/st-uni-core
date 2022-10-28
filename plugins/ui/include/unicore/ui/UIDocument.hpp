#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/ui/UINode.hpp"
#include "unicore/system/Event.hpp"

namespace unicore
{
	enum class UIEventType
	{
		Clicked,
		ValueChanged,
	};

	struct UIEvent
	{
		UINode node;
		UIEventType type;
		Variant value;
	};

	class UIDocument : public Resource
	{
		UC_OBJECT_EVENT(create_node, const UINode&);
		UC_OBJECT_EVENT(remove_node, const UINode&);

		UC_OBJECT_EVENT(change_index, const UINode& /* parent */, unsigned /* old */, unsigned /* new */);

		UC_OBJECT_EVENT(set_name, const UINode&, StringView);
		UC_OBJECT_EVENT(set_visible, const UINode&, Bool);
		UC_OBJECT_EVENT(set_attribute, const UINode&, UIAttributeType, const Optional<Variant>&);
		UC_OBJECT_EVENT(set_action, const UINode&, UIActionType, const Optional<UIAction>&);
	public:
		explicit UIDocument(Logger* logger = nullptr);

		UC_NODISCARD size_t get_system_memory_use() const override;

		Size get_roots(List<UINode>& list) const;
		UC_NODISCARD List<UINode> get_roots() const;

		// FIND //////////////////////////////////////////////////////////////////////
		UC_NODISCARD Optional<UINode> find_by_id(StringView id) const;

		UC_NODISCARD Optional<UINode> find_by_type(UINodeType type,
			const Optional<UINode>& parent = std::nullopt) const;
		Size find_all_by_type(UINodeType type, List<UINode>& list,
			const Optional<UINode>& parent = std::nullopt) const;

		UC_NODISCARD Optional<UINode> find_by_name(StringView name,
			const Optional<UINode>& parent = std::nullopt) const;
		Size find_all_by_name(StringView name, List<UINode>& list,
			const Optional<UINode>& parent = std::nullopt) const;

		UC_NODISCARD Optional<UINode> querry(
			const Predicate<const UINode&>& predicate,
			const Optional<UINode>& parent = std::nullopt) const;

		Size querry_all(
			const Predicate<const UINode&>& predicate, List<UINode>& list,
			const Optional<UINode>& parent = std::nullopt) const;

		// EVENTS ////////////////////////////////////////////////////////////////////
		void send_event(const UIEvent& evt);

		// CREATE ////////////////////////////////////////////////////////////////////
		Optional<UINode> create_node(UINodeType type, const UINodeOptions& options,
			const Optional<UINode>& parent = std::nullopt);

		Optional<UINode> duplicate(const UINode& node,
			const Optional<UINode>& at_parent = std::nullopt);

		Bool remove_node(const UINode& node);

		// VALUES ////////////////////////////////////////////////////////////////////
		UC_NODISCARD Bool is_node_valid(const UINode& node) const;

		UC_NODISCARD Bool get_node_uid(const UINode& node, String& value) const;

		UC_NODISCARD Bool get_node_name(const UINode& node, String& value) const;
		Bool set_node_name(const UINode& node, StringView name);

		UC_NODISCARD Bool get_node_visible(const UINode& node, Bool& value) const;
		Bool set_node_visible(const UINode& node, Bool value);

		UC_NODISCARD Bool get_node_type(const UINode& node, UINodeType& type) const;

		// HIERARCHY /////////////////////////////////////////////////////////////////
		UC_NODISCARD Optional<UINode> get_node_parent(const UINode& node) const;

		Size get_node_children(List<UINode>& list, const Optional<UINode>& node) const;
		UC_NODISCARD List<UINode> get_node_children(const Optional<UINode>& node) const;
		UC_NODISCARD Size get_node_children_count(const Optional<UINode>& node) const;

		UC_NODISCARD Optional<unsigned> get_node_sibling_index(const UINode& node) const;
		Bool set_node_sibling_index(const UINode& node, unsigned new_index);

		UC_NODISCARD Optional<UINode> get_node_next_sibling(const UINode& node) const;
		UC_NODISCARD Optional<UINode> get_node_prev_sibling(const UINode& node) const;

		// ATTRIBUTES ////////////////////////////////////////////////////////////////
		void set_node_attribute(const UINode& node,
			UIAttributeType type, const Optional<Variant>& value);

		UC_NODISCARD Optional<Variant> get_node_attribute(
			const UINode& node, UIAttributeType type) const;

		UC_NODISCARD Optional<UIAttributeDict> get_node_attributes(const UINode& node) const;
		UC_NODISCARD Bool get_node_attributes(const UINode& node, UIAttributeDict& dict) const;

		// ACTIONS ///////////////////////////////////////////////////////////////////
		void set_node_action(const UINode& node, UIActionType type, const Optional<UIAction>& action);

		UC_NODISCARD Optional<UIAction> get_node_action(const UINode& node, UIActionType type) const;

		UC_NODISCARD Optional<UIActionDict> get_node_actions(const UINode& node) const;
		UC_NODISCARD Bool get_node_actions(const UINode& node, UIActionDict& dict) const;

	protected:
		struct NodeInfo
		{
			UINodeType type = UINodeType::Group;
			String uid;
			String name;
			Bool visible = true;
			UINode::IndexType parent = UINode::InvalidIndex;
			List<UINode::IndexType> children;
			UIAttributeDict attributes;
			UIActionDict actions;
		};

		Logger* _logger;
		List<UINode::IndexType> _roots;
		Dictionary<UINode::IndexType, NodeInfo> _nodes;
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

		UINode::IndexType create_index();
		UC_NODISCARD UINode node_from_index(UINode::IndexType index) const;

		void internal_find_all_by_type(UINode::IndexType index,
			UINodeType type, List<UINode>& list, Size& count) const;
		void internal_find_all_by_name(UINode::IndexType index,
			StringView name, List<UINode>& list, Size& count) const;

		Optional<UINode> internal_querry(UINode::IndexType index,
			const Predicate<const UINode&>& predicate) const;

		void internal_querry_all(UINode::IndexType index,
			const Predicate<const UINode&>& predicate,
			List<UINode>& list, Size& count) const;

		Optional<UINode> internal_duplicate_recurse(const UINode& node, const Optional<UINode>& parent);
		void internal_remove_node_recurse(UINode::IndexType index, Size& count);

		static bool call_action_default(const UIAction& action, const UINode& node);
		static bool call_action_value(const UIAction& action, const UINode& node, const Variant& value);
	};
}