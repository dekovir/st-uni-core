#include "unicore/Font.hpp"
#include "unicore/Data.hpp"

namespace unicore
{
	size_t Font::system_memory_use() const
	{
		// TODO: Add kerning dictionary size;
		return sizeof(Font);
	}

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