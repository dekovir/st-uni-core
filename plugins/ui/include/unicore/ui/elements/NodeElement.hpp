#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	class NodeElement : public Element
	{
		UC_UI_ELEMENT_PROPERTY(name, String);
		UC_UI_ELEMENT_PROPERTY(hidden, Bool);
	};

	template<typename TValue>
	class ValueNodeElement : public NodeElement
	{
		UC_UI_ELEMENT_PROPERTY(value, TValue);
		UC_OBJECT_EVENT(changed, sfinae::ConstRefType<TValue>);
	public:
	};
}