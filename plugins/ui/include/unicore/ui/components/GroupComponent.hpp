#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	class GroupComponent : public Component
	{
	public:
		explicit GroupComponent(UIGroupType type);

		UC_NODISCARD Size size() const { return _children.size(); }

		template<typename T = Component,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(const Shared<T>& component)
		{
			// TODO: Create/destroy
			_children.push_back(component);
			if (is_mounted())
				component->mount(*document(), node());

			return component;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(const T& component)
		{
			return add(std::make_shared<T>(component));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(T&& component)
		{
			return add(std::make_shared<T>(std::forward<T>(component)));
		}

	protected:
		List<Shared<Component>> _children;

		void on_mount() override;
		void on_dismount() override;
	};

	template<UIGroupType Type>
	class TypedGroupComponent : public GroupComponent
	{
	public:
		TypedGroupComponent() : GroupComponent(Type) {}

		template<typename ... Args,
			std::enable_if_t<sfinae::all_is_component_v<Args...>>* = nullptr>
		explicit TypedGroupComponent(Args&&... args)
			: TypedGroupComponent()
		{
			((add(std::forward<Args>(args))), ...);
		}
	};

	using vlayout = TypedGroupComponent<UIGroupType::Vertical>;
	using hlayout = TypedGroupComponent<UIGroupType::Horizontal>;

	using child = TypedGroupComponent<UIGroupType::Child>;
	using list_box = TypedGroupComponent<UIGroupType::List>;
	using tree = TypedGroupComponent<UIGroupType::Tree>;
	using combo_group = TypedGroupComponent<UIGroupType::Combo>;
	using flex = TypedGroupComponent<UIGroupType::Flex>;

	using table_group = TypedGroupComponent<UIGroupType::Table>;
	using table_header = TypedGroupComponent<UIGroupType::TableHeader>;
	using table_row = TypedGroupComponent<UIGroupType::TableRow>;
	using table_cell = TypedGroupComponent<UIGroupType::TableCell>;

	using popup = TypedGroupComponent<UIGroupType::Popup>;
	using tooltip = TypedGroupComponent<UIGroupType::Tooltip>;
	using modal = TypedGroupComponent<UIGroupType::Modal>;
}