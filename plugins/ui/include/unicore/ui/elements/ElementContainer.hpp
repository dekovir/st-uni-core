#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	class ElementContainer : public Element
	{
	public:
		UC_NODISCARD Size size() const { return _elements.size(); }

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		auto add(const Shared<T>& element)
		{
			internal_add(element);
			return element;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		auto add(const T& element)
		{
			auto ptr = std::make_shared<T>(element);
			internal_add(ptr);
			return ptr;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		auto add(T&& element)
		{
			auto ptr = std::make_shared<T>(std::forward<T>(element));
			internal_add(ptr);
			return ptr;
		}

	protected:
		List<Shared<Element>> _elements;

		void did_update() override;

		void internal_add(const Shared<Element>& element);
	};
}