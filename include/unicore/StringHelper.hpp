#pragma once
#include "unicore/Defs.hpp"

namespace unicore::StringHelper
{
	extern int compare(const StringView a, const StringView b, bool case_sensitive);
	extern int compare(const WStringView a, const WStringView b, bool case_sensitive);

	extern String to_hex(intptr_t value);

	extern String print_format(StringView format, ...);

	template<typename Char>
	static bool ends_with(const BasicStringView<Char> string,
		const BasicStringView<Char> ending, bool case_sensitive)
	{
		if (string.length() >= ending.length())
		{
			const auto offset = string.length() - ending.length();
			return compare(
				string.data() + offset, ending.data(), case_sensitive) == 0;
		}

		return false;
	}
}