#pragma once
#include "unicore/ui/elements/TypedNodeElement.hpp"

namespace unicore::ui
{
	class TextElement : public TypedNodeElement<VisualText>
	{
		UC_UI_ELEMENT_PROPERTY(text, String32);
	public:
		TextElement() = default;

		explicit TextElement(StringView32 text);
		explicit TextElement(StringView text);

	protected:
		void apply_params(VisualText& item) override;
	};

	using bullet = TypedNodeElement<VisualBullet>;
	using separator = TypedNodeElement<VisualSeparator>;

	using text = TextElement;
}