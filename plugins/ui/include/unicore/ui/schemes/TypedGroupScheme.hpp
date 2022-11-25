#pragma once
#include "unicore/ui/schemes/TypedNodeScheme.hpp"

namespace unicore::ui
{
	template<UIGroupType Type, typename... TKeys>
	class TypedGroupScheme : public TypedNodeScheme<UINodeTag::Group, TKeys...>
	{
	public:
		using BaseClass = TypedNodeScheme<UINodeTag::Group, TKeys...>;

		template<typename ... Args,
			std::enable_if_t<all_is_scheme_v<Args...>>* = nullptr>
		explicit TypedGroupScheme(const typename BaseClass::Params& params, Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);

			((add(std::forward<Args>(args))), ...);
		}

		template<typename ... Args,
			std::enable_if_t<all_is_scheme_v<Args...>>* = nullptr>
		explicit TypedGroupScheme(Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;

			((add(std::forward<Args>(args))), ...);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
		auto add(const Shared<T>& item)
		{
			internal_add(item);
			return item;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
		auto add(const T& element)
		{
			auto item = std::make_shared<T>(element);
			internal_add(item);
			return item;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
		auto add(T&& element)
		{
			auto item = std::make_shared<T>(std::forward<T>(element));
			internal_add(item);
			return item;
		}

		UINode create(UIDocument& document, const UINode& parent) override
		{
			auto node = BaseClass::create(document, parent);

			for (const auto& element : _elements)
				element->create(document, node);

			return node;
		}

		Bool apply_to(UIDocument& document, const UINode& node) override
		{
			if (!BaseClass::apply_to(document, node))
				return false;

			// TODO: Optimize
			if (document.get_node_children_count(node) != _elements.size())
				return false;

			for (unsigned i = 0; i < _elements.size(); i++)
			{
				const auto& element = _elements[i];

				auto child = document.get_node_child(node, i);
				if (child.empty()) return false;

				if (!element->apply_to(document, child))
					return false;
			}

			return true;
		}

	protected:
		List<Shared<NodeScheme>> _elements;

		void internal_add(const Shared<NodeScheme>& element)
		{
			_elements.push_back(element);
		}
	};

	template<UIGroupType Type, typename ... TValues>
	using GroupTemplate = TypedGroupScheme<Type, TValues...>;

	using GroupV = GroupTemplate<UIGroupType::Vertical>;
	using GroupH = GroupTemplate<UIGroupType::Horizontal>;
	//using GroupChild = GroupTemplate<UIGroupType::Child>;
	using GroupList = GroupTemplate<UIGroupType::List>;
	using GroupTree = GroupTemplate<UIGroupType::Tree, attr::Value>;
	using GroupCombo = GroupTemplate<UIGroupType::Combo, attr::Value>;
	//using GroupFlex = GroupTemplate<UIGroupType::Flex>;

	using GroupTable = GroupTemplate<UIGroupType::Table, attr::Value>;
	using GroupTableHeader = GroupTemplate<UIGroupType::TableHeader, attr::Text>;
	using GroupTableRow = GroupTemplate<UIGroupType::TableRow>;
	using GroupTableCell = GroupTemplate<UIGroupType::TableCell, attr::Text>;

	using GroupPopup = GroupTemplate<UIGroupType::Popup, attr::Value>;
	using GroupTooltip = GroupTemplate<UIGroupType::Tooltip>;
	using GroupModal = GroupTemplate<UIGroupType::Modal, attr::Value>;
}