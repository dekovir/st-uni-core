#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/remoteui/Element.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/system/EnumFlag.hpp"

namespace unicore::remoteui
{
	struct UIEvent
	{
		Element node;
		UIActionType type;
		Variant value;
	};

	class Document : public Resource
	{
		UC_OBJECT_EVENT(create_node, const Element&);
		UC_OBJECT_EVENT(remove_node, const Element&);
		UC_OBJECT_EVENT(reorder_children, const Element&);
		UC_OBJECT_EVENT(set_attribute, const Element&, Attribute, const Variant&);
	public:
		explicit Document(Logger* logger = nullptr);

		UC_NODISCARD size_t get_system_memory_use() const override;

		Size get_roots(List<Element>& list) const;
		UC_NODISCARD List<Element> get_roots() const;

		// FIND ////////////////////////////////////////////////////////////////////
		UC_NODISCARD Element find_by_index(ElementIndex index) const;

		UC_NODISCARD Element find_by_tag(ElementTag tag,
			const Element& parent = Element::Empty) const;
		Size find_all_by_tag(ElementTag tag, List<Element>& list,
			const Element& parent = Element::Empty) const;

		UC_NODISCARD Element find_by_name(StringView name,
			const Element& parent = Element::Empty) const;
		Size find_all_by_name(StringView name, List<Element>& list,
			const Element& parent = Element::Empty) const;

		UC_NODISCARD Element query(
			const Predicate<const Element&>& predicate,
			const Element& parent = Element::Empty) const;

		Size query_all(const Predicate<const Element&>& predicate,
			List<Element>& list, const Element& parent = Element::Empty) const;

		// EVENTS //////////////////////////////////////////////////////////////////
		void send_event(const Element node, UIActionType type,
			const Variant& value = Variant::Empty);
		void send_event(const UIEvent& evt);

		// CREATE //////////////////////////////////////////////////////////////////
		Element create_node(ElementTag tag, const ElementOptions& options,
			const Element& parent = Element::Empty);

		Element create_group(GroupType type, const ElementOptions& options,
			const Element& parent = Element::Empty);
		Element create_visual(VisualType type, const ElementOptions& options,
			const Element& parent = Element::Empty);
		Element create_input(InputType type, const ElementOptions& options,
			const Element& parent = Element::Empty);

		Element duplicate(const Element& node, const Element& at_parent = Element::Empty);

		Bool remove_node(const Element& node);

		Bool apply_options(const Element& node, const ElementOptions& options);

		// VALUES //////////////////////////////////////////////////////////////////
		UC_NODISCARD Bool is_node_valid(const Element& node) const;
		UC_NODISCARD Bool get_node_tag(const Element& node, ElementTag& value) const;

		// HIERARCHY ///////////////////////////////////////////////////////////////
		UC_NODISCARD Element get_node_parent(const Element& node) const;

		Size get_node_children(List<Element>& list, const Element& node) const;
		UC_NODISCARD List<Element> get_node_children(const Element& node) const;
		UC_NODISCARD Element get_node_child(const Element& node, int index) const;
		UC_NODISCARD Size get_node_children_count(const Element& node) const;

		UC_NODISCARD int get_node_sibling_index(const Element& node) const;
		Bool set_node_sibling_index(const Element& node, int new_index);

		UC_NODISCARD Element get_node_next_sibling(const Element& node) const;
		UC_NODISCARD Element get_node_prev_sibling(const Element& node) const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		void set_node_attribute(const Element& node,
			Attribute attribute, const Variant& value);
		UC_NODISCARD const Variant& get_node_attribute(const Element& node, Attribute attribute) const;

		UC_NODISCARD Optional<AttributeDict> get_node_attributes(const Element& node) const;
		UC_NODISCARD Bool get_node_attributes(const Element& node, AttributeDict& dict) const;

		UC_NODISCARD StringView get_node_name(const Element& node) const;
		void set_node_name(const Element& node, StringView value);

		UC_NODISCARD Bool get_node_hidden(const Element& node) const;
		void set_node_hidden(const Element& node, Bool value);

		// ACTIONS /////////////////////////////////////////////////////////////////
		void subscribe_node(const Element& node, UIActionType type, const UIAction& action);
		Bool unsubscribe_node(const Element& node, UIActionType type);

	protected:
		struct NodeInfo
		{
			ElementTag tag = ElementTag::Group;
			// UInt16 sibling_index; ? Optimization
			ElementIndex parent = ElementIndex_Invalid;
			AttributeDict attributes;
		};

		using NodeIndexList = List<ElementIndex>;

		Logger* _logger;
		NodeIndexList _roots;

		// TODO: Optimize with list
		Dictionary<ElementIndex, NodeInfo> _node_infos;
		Dictionary<ElementIndex, UIActionDict> _node_actions;
		Dictionary<ElementIndex, NodeIndexList> _node_children;

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

		ElementIndex::TypeValue _last_index = 0;

		NodeInfo* get_info(ElementIndex index);
		UC_NODISCARD const NodeInfo* get_info(ElementIndex index) const;

		NodeInfo* get_info(const Element& node);
		UC_NODISCARD const NodeInfo* get_info(const Element& node) const;

		UIActionDict* get_actions(ElementIndex index);
		UC_NODISCARD const UIActionDict* get_actions(ElementIndex index) const;

		NodeIndexList* get_children_list(ElementIndex index);
		UC_NODISCARD const NodeIndexList* get_children_list(ElementIndex index) const;

		ElementIndex create_index();
		UC_NODISCARD Element node_from_index(ElementIndex index) const;

		void internal_find_all_by_tag(ElementIndex index,
			ElementTag tag, List<Element>& list, Size& count) const;
		void internal_find_all_by_name(ElementIndex index,
			StringView name, List<Element>& list, Size& count) const;

		Element internal_query(ElementIndex index,
			const Predicate<const Element&>& predicate) const;

		void internal_query_all(ElementIndex index,
			const Predicate<const Element&>& predicate,
			List<Element>& list, Size& count) const;

		ElementIndex internal_create_node(ElementTag tag,
			const ElementOptions& options, ElementIndex parent);
		ElementIndex internal_duplicate(const Element& node, ElementIndex parent);
		void internal_remove_node(ElementIndex index, Size& count);

		static bool call_action_default(const UIAction& action, const Element& node);
		static bool call_action_value(const UIAction& action, const Element& node, const Variant& value);
	};
}