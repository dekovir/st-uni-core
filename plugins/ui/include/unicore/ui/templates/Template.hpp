#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "unicore/system/StrongType.hpp"

namespace unicore::ui
{
	class Template
	{
	public:
		virtual ~Template() = default;

		virtual UINode create(UIDocument& document, const UINode& parent) = 0;
		virtual Bool apply_to(UIDocument& document, const UINode& node) = 0;
	};

	namespace attr
	{
		UNICORE_MAKE_STRONG_TYPE(Uid, String);
		UNICORE_MAKE_STRONG_TYPE(Name, String);
		UNICORE_MAKE_STRONG_TYPE(Visible, Bool);

		UNICORE_MAKE_STRONG_TYPE(Type, UInt8);
		UNICORE_MAKE_STRONG_TYPE(Value, Variant);

		UNICORE_MAKE_STRONG_TYPE(Width, Variant);
		UNICORE_MAKE_STRONG_TYPE(Height, Variant);

		UNICORE_MAKE_STRONG_TYPE(Tooltip, String32);
		UNICORE_MAKE_STRONG_TYPE(Text, String32);

		UNICORE_MAKE_STRONG_TYPE(Step, Variant);
		UNICORE_MAKE_STRONG_TYPE(Min, Variant);
		UNICORE_MAKE_STRONG_TYPE(Max, Variant);
	}

	namespace action
	{
		UNICORE_MAKE_STRONG_TYPE(OnClick, UIActionDefault);
		UNICORE_MAKE_STRONG_TYPE(OnChange, UIAction);
	}

	// SFINAE ////////////////////////////////////////////////////////////////////
	template <class T>
	inline constexpr bool is_attr_v =
		std::is_same_v<T, attr::Uid> ||
		std::is_same_v<T, attr::Name> ||
		std::is_same_v<T, attr::Visible> ||
		std::is_same_v<T, attr::Type> ||
		std::is_same_v<T, attr::Value> ||
		std::is_same_v<T, attr::Width> ||
		std::is_same_v<T, attr::Height> ||
		std::is_same_v<T, attr::Tooltip> ||
		std::is_same_v<T, attr::Text> ||
		std::is_same_v<T, attr::Step> ||
		std::is_same_v<T, attr::Min> ||
		std::is_same_v<T, attr::Max>;

	template<typename ... T>
	inline constexpr Bool all_is_attr_v = (... && is_attr_v<T>);

	template <class T>
	inline constexpr bool is_action_v =
		std::is_same_v<T, action::OnClick> ||
		std::is_same_v<T, action::OnChange>;

	template<typename ... T>
	inline constexpr Bool all_is_action_v = (... && is_action_v<T>);

	template <class T>
	inline constexpr bool is_attr_or_action_v =
		is_attr_v<T> || is_action_v<T>;

	template<typename ... T>
	inline constexpr Bool all_is_attr_or_action_v = (... && is_attr_or_action_v<T>);

	// UTILITIES /////////////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(const T& element)
	{
		return std::make_shared<T>(element);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(const T& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}
}