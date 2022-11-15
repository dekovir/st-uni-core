#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	template<typename T>
	class TypedComponent : public Component
	{
	public:
		explicit TypedComponent(UINodeTag tag) : Component(tag) {}
		TypedComponent(UINodeTag tag, T value) : TypedComponent(tag) { set_value(value); }

		void set_value(T value) { set_attribute(UIAttribute::Value, value); }
		UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }
	};

	class ItemComponent : public TypedComponent<Bool>
	{
	public:
		explicit ItemComponent(StringView32 text);

		ItemComponent(StringView32 text, Bool value);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;

		void set_click_action(const UIAction& action);
	};

	using item = ItemComponent;
}