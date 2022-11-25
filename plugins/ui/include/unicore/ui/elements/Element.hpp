#pragma once
#include "unicore/ui/UITemplate.hpp"

namespace unicore::ui
{
	UNICORE_MAKE_INDEX_WITH_INVALID(ElementIndex, UInt16);

	class ElementContainer;

	class Element
	{
	public:
		Element();
		virtual ~Element() = default;

		UC_NODISCARD ElementIndex index() const { return _index; }
		UC_NODISCARD ElementContainer* parent() const { return _parent; }

		void set_parent(ElementContainer* parent);

		virtual Shared<Template> render() = 0;

	protected:
		const ElementIndex _index;
		ElementContainer* _parent = nullptr;

		void rebuild();

		friend class ElementContainer;
	};

	// SFINAE ////////////////////////////////////////////////////////////////////
	template<typename T>
	inline constexpr Bool is_element_v =
		std::is_base_of_v<Element, T> ||
		std::is_convertible_v<T, Shared<Element>>;

	template<typename ... T>
	inline constexpr Bool all_is_element_v = (... && is_element_v<T>);

	// UTILITIES /////////////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ptr(const T& item)
	{
		return std::make_shared<T>(item);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ptr(T&& item)
	{
		return std::make_shared<T>(std::forward<T>(item));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ref(const T& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
	extern Shared<T> ref(T&& item, Shared<T>& ref)
	{
		ref = ptr(item);
		return ref;
	}
}