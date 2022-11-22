#pragma once
#include "unicore/ui/components/InputComponent.hpp"

namespace unicore::ui
{
	class ItemComponent : public TypedInputComponent<Bool>
	{
		UC_OBJECT_EVENT(clicked, Bool);
	public:
		explicit ItemComponent(StringView32 text);

		ItemComponent(StringView32 text, Bool value);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;

		void set_click_action(const Event_clicked::ActionType& action);
	};

	using item = ItemComponent;
}