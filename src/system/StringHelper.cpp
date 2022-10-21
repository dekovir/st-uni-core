#include "unicore/system/StringHelper.hpp"

#include <cstdarg>

namespace unicore::StringHelper
{
	constexpr size_t s_size = 1024 * 2;
	static char s_buffer[s_size];

	constexpr char s_hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	String to_hex(intptr_t value)
	{
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
}