#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	class ElementContainer : public Element
	{
	public:
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

		Bool rebuild_element(const Element& element);

	protected:
		struct Info
		{
			Shared<Element> element;
			UINode node = UINode::Empty;
		};

		Shared<UIDocument> _document;
		List<ElementIndex> _indexes;
		Dictionary<ElementIndex, Info> _infos;

		void internal_add(const Shared<Element>& element);

		Bool internal_rebuild(Info& info);

		virtual UINode get_container_node() = 0;
	};
}