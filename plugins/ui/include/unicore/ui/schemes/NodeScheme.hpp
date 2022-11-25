#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "unicore/ui/UINodeValues.hpp"

namespace unicore::ui
{
	class NodeScheme
	{
	public:
		virtual ~NodeScheme() = default;

		virtual UINode create(UIDocument& document, const UINode& parent) = 0;
		virtual Bool apply_to(UIDocument& document, const UINode& node) = 0;
	};

	// SFINAE ////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Bool is_scheme_v =
		std::is_base_of_v<NodeScheme, T> ||
		std::is_convertible_v<T, Shared<NodeScheme>>;

	template<typename ... T>
	inline constexpr Bool all_is_scheme_v = (... && is_scheme_v<T>);

	// UTILITIES /////////////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
	extern Shared<T> ptr(const T& item)
	{
		return std::make_shared<T>(item);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
	extern Shared<T> ptr(T&& item)
	{
		return std::make_shared<T>(std::forward<T>(item));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
	extern Shared<T> ref(const T& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<NodeScheme, T>>* = nullptr>
	extern Shared<T> ref(T&& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}
}