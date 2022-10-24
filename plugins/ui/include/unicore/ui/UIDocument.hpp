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

	using UIEventValue = StdVariant<UIActionType, Double, String, String32>;

	struct UIEvent
	{
		UINode node;
		UIEventType type;
		// TODO: Replace to Variant
		UIEventValue value;
	};

	class UIDocument
	{
		UC_OBJECT_EVENT(create_node, const UINode&);
		UC_OBJECT_EVENT(set_attribute, const UINode&, UIAttributeType, const Optional<Variant>&);
		UC_OBJECT_EVENT(set_action, const UINode&, UIActionType, const Optional<UIAction>&);
	public:
		explicit UIDocument(Logger* logger = nullptr);

		size_t get_root_nodes(List<UINode>& nodes);
		UC_NODISCARD List<UINode> get_root_nodes();

		// FIND //////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINodeIndex find_index_by_id(StringView id) const;
		UC_NODISCARD Optional<UINode> find_node_by_id(StringView id);

		size_t find_indexes_by_name(StringView name,
			List<UINodeIndex>& list, UINodeIndex parent) const;
		size_t find_nodes_by_name(StringView name,
			List<UINode>& list, UINodeIndex parent);

		size_t find_indexes_by_name_recurse(StringView name,
			List<UINodeIndex>& list, UINodeIndex parent) const;
		size_t find_nodes_by_name_recurse(StringView name,
			List<UINode>& list, UINodeIndex parent);

		UC_NODISCARD UINodeIndex find_index_by_name(
			StringView name, UINodeIndex parent) const;
		Optional<UINode> find_node_by_name(StringView name, UINodeIndex parent);

		UC_NODISCARD UINodeIndex find_index_by_name_recurse(
			StringView name, UINodeIndex parent) const;
		Optional<UINode> find_node_by_name_recurse(StringView name, UINodeIndex parent);

		// EVENTS ////////////////////////////////////////////////////////////////////
		void send_event(const UIEvent& evt);

		// RAW INDEX /////////////////////////////////////////////////////////////////
		UINodeIndex create_node(UINodeType type, UINodeIndex parent,
			const UIAttributes& attributes = {},
			const UINodeActions& actions = {});

		UC_NODISCARD Bool is_node_valid(UINodeIndex index) const;
		UC_NODISCARD UINodeType get_node_type(UINodeIndex index) const;
		UC_NODISCARD UINodeIndex get_node_parent(UINodeIndex index) const;
		UC_NODISCARD const List<UINodeIndex>& get_node_children(UINodeIndex index) const;

		UC_NODISCARD UINodeIndex get_node_next_sibling(UINodeIndex index) const;
		UC_NODISCARD UINodeIndex get_node_prev_sibling(UINodeIndex index) const;

		void set_node_attribute(UINodeIndex index,
			UIAttributeType type, const Optional<Variant>& value);

		UC_NODISCARD Optional<Variant> get_node_attribute(
			UINodeIndex index, UIAttributeType type) const;

		UC_NODISCARD const UIAttributes& get_node_attributes(UINodeIndex index) const;

		void set_node_action(UINodeIndex index, UIActionType type, const Optional<UIAction>& action);

		UC_NODISCARD const UINodeActions& get_node_actions(UINodeIndex index) const;

	protected:
		Logger* _logger;
		List<UINodeIndex> _roots;
		Dictionary<String, UINodeIndex> _id_dict;

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