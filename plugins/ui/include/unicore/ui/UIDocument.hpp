#pragma once
#include "unicore/ui/UINode.hpp"
#include "unicore/system/Event.hpp"

namespace unicore
{
	enum class UIEventType
	{
		ActionCall,
		ValueChanged,
	};

	using UIEventValue = Variant<UIActionType, Double, String, String32>;

	struct UIEvent
	{
		UINode node;
		UIEventType type;
		UIEventValue value;
	};

	class UIDocument
	{
		UC_OBJECT_EVENT(create_node, const UINode&);
		UC_OBJECT_EVENT(set_attribute, const UINode&, UIAttributeType, const Optional<UIAttributeValue>&);
		UC_OBJECT_EVENT(set_action, const UINode&, UIActionType, const Optional<UIAction>&);
	public:
		explicit UIDocument(Logger* logger = nullptr);

		size_t get_root_nodes(List<UINode>& nodes);
		UC_NODISCARD List<UINode> get_root_nodes();

		// EVENTS ////////////////////////////////////////////////////////////////////
		void send_event(const UIEvent& evt);

		// RAW INDEX /////////////////////////////////////////////////////////////////
		size_t get_root_indices(List<UINodeIndex>& indices) const;
		UC_NODISCARD List<UINodeIndex> get_root_indices() const;

		UINodeIndex create_node(UINodeType type, UINodeIndex parent,
			const UIAttributes& attributes = {},
			const UINodeActions& actions = {});

		UC_NODISCARD UINodeType get_node_type(UINodeIndex index) const;
		UC_NODISCARD UINodeIndex get_node_parent(UINodeIndex index) const;
		UC_NODISCARD const List<UINodeIndex>& get_node_children(UINodeIndex index) const;

		UC_NODISCARD UINodeIndex get_node_next_sibling(UINodeIndex index) const;
		UC_NODISCARD UINodeIndex get_node_prev_sibling(UINodeIndex index) const;

		void set_node_attribute(UINodeIndex index,
			UIAttributeType type, const Optional<UIAttributeValue>& value);

		UC_NODISCARD Optional<UIAttributeValue> get_node_attribute(
			UINodeIndex index, UIAttributeType type) const;

		UC_NODISCARD const UIAttributes& get_node_attributes(UINodeIndex index) const;

		void set_node_action(UINodeIndex index, UIActionType type, const Optional<UIAction>& action);

		UC_NODISCARD const UINodeActions& get_node_actions(UINodeIndex index) const;

	protected:
		Logger* _logger;
		UINodeIndex _root_index = UINodeIndexInvalid;

		struct NodeInfo
		{
			UINodeType type;
			UINodeIndex parent;
			List<UINodeIndex> children;
			UIAttributes attributes;
			UINodeActions actions;
		};

		List<NodeInfo> _nodes;

		NodeInfo* get_info(UINodeIndex index);
		UC_NODISCARD const NodeInfo* get_info(UINodeIndex index) const;
	};
}