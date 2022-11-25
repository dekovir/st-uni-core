#pragma once
#include "unicore/ui/elements/TypedElement.hpp"

namespace unicore::ui
{
	class TextElement : public TypedElement<VisualText>
	{
		UC_UI_ELEMENT_PROPERTY(text, String32);
	public:
		TextElement() = default;

		explicit TextElement(StringView32 text)
			: _text(text)
		{}

	protected:
		void apply_params(VisualText& item) override
		{
			item.set_params({ attr::Text(_text) });
		}
	};

	using bullet = TypedElement<VisualBullet>;
	using separator = TypedElement<VisualSeparator>;

	using text = TextElement;
}