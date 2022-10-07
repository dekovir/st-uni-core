#include "unicore/Font.hpp"

namespace unicore
{
	Vector2f Font::calc_size(StringView32 text) const
	{
		return { calc_width(text), get_height() };
	}
}