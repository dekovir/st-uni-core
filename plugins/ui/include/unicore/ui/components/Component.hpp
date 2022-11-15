#pragma once
#include "unicore/ui/UIDocument.hpp"

namespace unicore::ui
{
	class Component
	{
	public:
		explicit Component(UINodeTag tag);
		virtual ~Component() = default;

		UC_NODISCARD Bool is_mounted() const { return !_node.empty(); }

		UC_NODISCARD UINodeTag tag() const { return _tag; }
		UC_NODISCARD const UINode& node() const { return _node; }

		UC_NODISCARD UIDocument* document() { return _document; }
		UC_NODISCARD const UIDocument* document() const { return _document; }

		void mount(UIDocument& document, const UINode& parent);
		void dismount();

	protected:
		virtual void on_mount() {}
		virtual void on_dismount() {}

		UC_NODISCARD Variant get_attribute(UIAttribute type) const { return _node.get(type); }

		void set_attribute(UIAttribute type, const Variant& value);
		void set_action(UIActionType type, const UIAction& action);

	private:
		const UINodeTag _tag;
		UINodeOptions _options;

		UIDocument* _document = nullptr;
		UINode _node = UINode::Empty;
	};

	using component = Component;

	namespace sfinae
	{
		template<typename T>
		inline constexpr Bool is_component_v =
			std::is_base_of_v<Component, T> ||
			std::is_convertible_v<T, Shared<Component>>;

		template<typename ... T>
		inline constexpr Bool all_is_component_v = (... && is_component_v<T>);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern auto ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern auto ptr(const Shared<T>& element)
	{
		return element;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& value)
	{
		value = std::make_shared<T>(std::forward<T>(element));
		return value;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ref(const Shared<T>& element, Shared<T>& value)
	{
		value = element;
		return value;
	}
}