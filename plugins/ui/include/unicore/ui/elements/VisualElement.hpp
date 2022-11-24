#pragma once
#include "unicore/ui/elements/TypedElement.hpp"

namespace unicore::ui
{
	class TextElement : public TypedElement<VisualText>
	{
	public:
		TextElement() = default;

		explicit TextElement(StringView32 text)
			: _text(text)
		{}

		void set_text(StringView32 text)
		{
			if (_text == text) return;

			_text = text;
			rebuild();
		}

	protected:
		String32 _text;

		void apply_params(VisualText& item) override
		{
			item.set_params({ attr::Text(_text) });
		}
	};

	using bullet = TypedElement<VisualBullet>;
	using separator = TypedElement<VisualSeparator>;

	using text = TextElement;
}