#pragma once
#include "unicore/ui/templates/TypedTemplate.hpp"

namespace unicore::ui
{
	template<UIGroupType Type, typename... TKeys>
	class TypedGroupTemplate : public TypedTemplate<UINodeTag::Group, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Group, TKeys...>;

		template<typename ... Args>
		explicit TypedGroupTemplate(const typename BaseClass::Params& params, Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);

			((add(std::forward<Args>(args))), ...);
		}

		template<typename ... Args>
		explicit TypedGroupTemplate(Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;

			((add(std::forward<Args>(args))), ...);
		}

		void add(const Shared<Template>& element)
		{
			_elements.push_back(element);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
		void add(const T& element)
		{
			add(std::make_shared<T>(element));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
		void add(T&& element)
		{
			add(std::make_shared<T>(std::forward<T>(element)));
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
		List<Shared<Template>> _elements;
	};

	template<UIGroupType Type, typename ... TValues>
	using GroupTemplate = TypedGroupTemplate<Type, TValues...>;

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