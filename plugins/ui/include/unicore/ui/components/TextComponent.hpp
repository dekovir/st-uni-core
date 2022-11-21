#pragma once
#include "unicore/ui/components/VisualComponent.hpp"

namespace unicore::ui
{
	class TextComponent : public VisualComponent
	{
	public:
		TextComponent();

		explicit TextComponent(StringView32 text);

		void set_text(StringView32 text);
		UC_NODISCARD String32 text() const;
	};

	using text = TextComponent;
}