#include "unicore/Font.hpp"
#include "unicore/Data.hpp"

namespace unicore
{
	int Font::find_kerning(uint32_t a, uint32_t b) const
	{
		if (const auto it = kerning.find(a); it != kerning.end())
		{
			if (const auto jt = it->second.find(b); jt != it->second.end())
				return jt->second;
		}

		return 0;
	}
}