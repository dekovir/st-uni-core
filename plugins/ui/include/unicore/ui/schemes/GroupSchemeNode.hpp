#pragma once
#include "unicore/ui/schemes/SchemeNode.hpp"

namespace unicore::ui
{
	class GroupSchemeNode : public SchemeNode
	{
	public:
		explicit GroupSchemeNode(UIGroupType type);

		UINode create(UIDocument& document, const UINode& parent) const override;
		Bool apply_to(UIDocument& document, const UINode& node) const override;

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
		auto add(const Shared<T>& node)
		{
			internal_add(node);
			return node;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
		auto add(const T& node)
		{
			auto item = std::make_shared<T>(node);
			internal_add(item);
			return item;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<SchemeNode, T>>* = nullptr>
		auto add(T&& node)
		{
			auto item = std::make_shared<T>(std::forward<T>(node));
			internal_add(item);
			return item;
		}

	protected:
		const UIGroupType _type;
		List<Shared<SchemeNode>> _children;

		void fill_optons(UINodeOptions& options) const override;

		void internal_add(const Shared<SchemeNode>& node);
	};

	template<UIGroupType Type, typename ... Keys>
	class GroupSchemeNodeT : public GroupSchemeNode, public SchemeNodeKeys<Keys...>
	{
	public:
		using Params = typename SchemeNodeKeys<Keys...>::Params;

		GroupSchemeNodeT() : GroupSchemeNode(Type) {}

		template<typename ... Args,
			std::enable_if_t<all_is_scheme_node_v<Args...>>* = nullptr>
		explicit GroupSchemeNodeT(Args&&... args)
			: GroupSchemeNode(Type)
		{
			((add(std::forward<Args>(args))), ...);
		}

		template<typename ... Args,
			std::enable_if_t<all_is_scheme_node_v<Args...>>* = nullptr>
		explicit GroupSchemeNodeT(Params params, Args&&... args)
			: GroupSchemeNode(Type)
		{
			SchemeNodeKeys<Keys...>::_values.apply(params);

			((add(std::forward<Args>(args))), ...);
		}

	protected:
		void fill_optons(UINodeOptions& options) const override
		{
			SchemeNodeKeys<Keys...>::_values.fill(options);
			GroupSchemeNode::fill_optons(options);
		}
	};

	using GroupV = GroupSchemeNodeT<UIGroupType::Vertical>;
	using GroupH = GroupSchemeNodeT<UIGroupType::Horizontal>;
	//using GroupChild = GroupSchemeNodeT<UIGroupType::Child>;
	using GroupList = GroupSchemeNodeT<UIGroupType::List>;
	using GroupTree = GroupSchemeNodeT<UIGroupType::Tree, attr::Value>;
	using GroupCombo = GroupSchemeNodeT<UIGroupType::Combo, attr::Value>;
	//using GroupFlex = GroupSchemeNodeT<UIGroupType::Flex>;

	using GroupTable = GroupSchemeNodeT<UIGroupType::Table, attr::Value>;
	using GroupTableHeader = GroupSchemeNodeT<UIGroupType::TableHeader, attr::Text>;
	using GroupTableRow = GroupSchemeNodeT<UIGroupType::TableRow>;
	using GroupTableCell = GroupSchemeNodeT<UIGroupType::TableCell, attr::Text>;

	using GroupPopup = GroupSchemeNodeT<UIGroupType::Popup, attr::Value>;
	using GroupTooltip = GroupSchemeNodeT<UIGroupType::Tooltip>;
	using GroupModal = GroupSchemeNodeT<UIGroupType::Modal, attr::Value>;
}