#include "unicore/ui/components/TextComponent.hpp"

namespace unicore::ui
{
	TextComponent::TextComponent()
		: Component(UINodeTag::Text)
	{
	}

	TextComponent::TextComponent(StringView32 text)
		: TextComponent()
	{
		set_attribute(UIAttribute::Text, text);
	}

	void TextComponent::set_text(StringView32 text)
	{
		set_attribute(UIAttribute::Text, text);
	}

	String32 TextComponent::text() const
	{
		return get_attribute(UIAttribute::Text).get_string32();
	}
}