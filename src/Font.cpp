#include "unicore/Font.hpp"

namespace unicore
{
	Vector2f Font::calc_size(WStringView text) const
	{
		return { calc_width(text), get_height() };
	}
}