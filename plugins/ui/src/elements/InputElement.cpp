#include "unicore/ui/elements/InputElement.hpp"
#include "unicore/system/Unicode.hpp"

namespace unicore::ui
{
	ItemElement::ItemElement(StringView32 text)
		: _text(text)
	{}

	ItemElement::ItemElement(StringView text)
		: _text(Unicode::to_utf32(text))
	{
	}

	void ItemElement::apply_params(InputItem& item)
	{
		ValueInputElement<InputItem, Bool>::apply_params(item);

		if (!_text.empty())
			item.set_params({ attr::Text(_text) });

		if (!_event_clicked.empty())
			item.set_params({ attr::OnClick([&] { _event_clicked.invoke(); }) });
	}
}