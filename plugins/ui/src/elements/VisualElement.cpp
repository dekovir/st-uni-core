#include "unicore/ui/elements/VisualElement.hpp"
#include "unicore/system/Unicode.hpp"

namespace unicore::ui
{
	TextElement::TextElement(StringView32 text)
		: _text(text)
	{}

	TextElement::TextElement(StringView text)
		: _text(Unicode::to_utf32(text))
	{
	}

	void TextElement::apply_params(VisualText& item)
	{
		TypedNodeElement<VisualText>::apply_params(item);

		item.set_params({ attr::Text(_text) });
	}
}