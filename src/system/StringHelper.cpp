#include "unicore/system/StringHelper.hpp"
#include "unicore/system/AsciiTable.hpp"
#include "unicore/system/UnicodeTable.hpp"
#include <cstdarg>

namespace unicore::StringHelper
{
	constexpr size_t s_size = 1024 * 2;
	static char s_buffer[s_size];

	constexpr char s_hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	String to_hex(intptr_t value)
	{
		// TODO: Make constexpr
		String str(sizeof(value) * 2, ' ');

		for (unsigned i = 0; i < sizeof(value); i++)
		{
			const uint8_t b = (value >> (i * 8)) & 0xFF;
			str[i * 2 + 0] = s_hexmap[b & 0xF];
			str[i * 2 + 1] = s_hexmap[b >> 4];
		}

		std::reverse(str.begin(), str.end());

		while (str.size() > 1 && str[0] == '0')
			str.erase(str.begin());

		return str;
	}

	String print_format(StringView format, ...)
	{
		va_list args;
		va_start(args, format);
		vsprintf(s_buffer, format.data(), args);
		va_end(args);
		return s_buffer;
	}

	Char to_lower(Char c)
	{
		return AsciiTable::to_lower(c);
	}

	Char32 to_lower(Char32 c)
	{
		return UnicodeTable::to_lower(c);
	}

	String to_lower(StringView str)
	{
		String a(str);
		std::for_each(a.begin(), a.end(), [](auto& c) { c = to_lower(c); });
		return a;
	}

	String32 to_lower(StringView32 str)
	{
		String32 a(str);
		std::for_each(a.begin(), a.end(), [](auto& c) { c = to_lower(c); });
		return a;
	}

	Char to_upper(Char c)
	{
		return AsciiTable::to_upper(c);
	}

	Char32 to_upper(Char32 c)
	{
		return UnicodeTable::to_upper(c);
	}

	String to_upper(StringView str)
	{
		String a(str);
		std::for_each(a.begin(), a.end(), [](auto& c) { c = to_upper(c); });
		return a;
	}

	String32 to_upper(StringView32 str)
	{
		String32 a(str);
		std::for_each(a.begin(), a.end(), [](auto& c) { c = to_upper(c); });
		return a;
	}

	Int compare(StringView a, StringView b, bool case_insensetive)
	{
		if (a.size() < b.size()) return -1;
		if (a.size() > b.size()) return +1;

		if (!case_insensetive)
		{
			for (unsigned i = 0; i < a.size(); i++)
			{
				if (a[i] < b[i]) return -1;
				if (a[i] > b[i]) return -1;
			}
		}
		else
		{
			for (unsigned i = 0; i < a.size(); i++)
			{
				const auto a_c = to_lower(a[i]);
				const auto b_c = to_lower(b[i]);
				if (a_c < b_c) return -1;
				if (a_c > b_c) return -1;
			}
		}

		return 0;
	}

	Int compare(StringView32 a, StringView32 b, bool case_insensetive)
	{
		if (a.size() < b.size()) return -1;
		if (a.size() > b.size()) return +1;

		if (!case_insensetive)
		{
			for (unsigned i = 0; i < a.size(); i++)
			{
				if (a[i] < b[i]) return -1;
				if (a[i] > b[i]) return -1;
			}
		}
		else
		{
			for (unsigned i = 0; i < a.size(); i++)
			{
				const auto a_c = to_lower(a[i]);
				const auto b_c = to_lower(b[i]);
				if (a_c < b_c) return -1;
				if (a_c > b_c) return -1;
			}
		}

		return 0;
	}

	Bool equals(StringView a, StringView b, bool case_insensetive)
	{
		return compare(a, b, case_insensetive) == 0;
	}

	Bool equals(StringView32 a, StringView32 b, bool case_insensetive)
	{
		return compare(a, b, case_insensetive) == 0;
	}
}