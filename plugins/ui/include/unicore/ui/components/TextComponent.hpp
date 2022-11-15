#pragma once
#include "unicore/ui/components/Component.hpp"

namespace unicore::ui
{
	class TextComponent : public Component
	{
	public:
		TextComponent();

		explicit TextComponent(StringView32 text);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;
	};

	using text = TextComponent;
}