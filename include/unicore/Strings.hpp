#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	namespace Strings
	{
		extern bool try_convert(const WStringView from, String& to);
		extern bool try_convert(const StringView from, WString& to);

		extern String to_utf8(const WStringView str, bool* success = nullptr);
		extern WString to_wcs(const StringView str, bool* success = nullptr);

		extern int compare(const StringView a, const StringView b, bool case_sensitive);
		extern int compare(const WStringView a, const WStringView b, bool case_sensitive);

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
}
