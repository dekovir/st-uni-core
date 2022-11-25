#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
#define UC_UI_ELEMENT_PROPERTY(name, type) \
	public: UC_NODISCARD sfinae::ConstRefType<type> name() const { return _ ## name; } \
	void set_ ## name(sfinae::ConstRefType<type> value) \
	{ if (_ ## name == value) return; _ ## name = value; rebuild(); } \
	protected: type _ ## name = {}

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