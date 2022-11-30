#include "unicore/system/Unicode.hpp"
#include <codecvt>
#include <locale>

namespace unicore::Unicode
{
	bool try_convert(StringView32 from, String16& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
			std::string bytes = conv.to_bytes(&from.front(), &from.back() + 1);
			to = std::u16string(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length() / sizeof(char16_t));
		}
		else
		{
			to = u"";
		}

		return true;
	}

	bool try_convert(StringView32 from, StringW& to)
	{
		if (!from.empty())
		{
			String utf8;
			return
				try_convert(from, utf8) &&
				try_convert(utf8, to);
		}

		to = L"";
		return true;
	}

	bool try_convert(StringView32 from, String& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			to = conv.to_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = "";
		}
		return true;
	}

	bool try_convert(StringView16 from, String32& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
			to = conv.from_bytes(reinterpret_cast<const char*>(from.data()), reinterpret_cast<const char*>(from.data() + from.size()));
		}
		else
		{
			to = U"";
		}
		return true;
	}

	bool try_convert(StringView16 from, StringW& to)
	{
		if (!from.empty())
		{
			String str;
			return
				try_convert(from, str) &&
				try_convert(str, to);
		}

		to = L"";
		return true;
	}

	bool try_convert(StringView16 from, String& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
			to = conv.to_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = "";
		}
		return true;
	}

	bool try_convert(StringViewW from, String32& to)
	{
		if (!from.empty())
		{
			String str;
			return
				try_convert(from, str) &&
				try_convert(str, to);
		}

		to = U"";
		return true;
	}

	bool try_convert(StringViewW from, String16& to)
	{
		if (!from.empty())
		{
			String str;
			return
				try_convert(from, str) &&
				try_convert(str, to);
		}

		to = u"";
		return true;
	}

	bool try_convert(StringViewW from, String& to)
	{
		if (!from.empty())
		{
			static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
			to = conv.to_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = "";
		}
		return true;
	}

	bool try_convert(StringView from, String32& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
			to = conv.from_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = U"";
		}
		return true;
	}

	bool try_convert(StringView from, String16& to)
	{
		if (!from.empty())
		{
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
			to = conv.from_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = u"";
		}
		return true;
	}

	bool try_convert(StringView from, StringW& to)
	{
		if (!from.empty())
		{
			static std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > conv;
			to = conv.from_bytes(&from.front(), &from.back() + 1);
		}
		else
		{
			to = L"";
		}
		return true;
	}

	String32 to_utf32(StringView from, StringView32 default_value)
	{
		String32 to;
		return try_convert(from, to) ? to : String32(default_value);
	}

	String32 to_utf32(StringView16 from, StringView32 default_value)
	{
		String32 to;
		return try_convert(from, to) ? to : String32(default_value);
	}

	String32 to_utf32(StringViewW from, StringView32 default_value)
	{
		String32 to;
		return try_convert(from, to) ? to : String32(default_value);
	}

	String16 to_utf16(StringView32 from, StringView16 default_value)
	{
		String16 to;
		return try_convert(from, to) ? to : String16(default_value);
	}

	String16 to_utf16(StringViewW from, StringView16 default_value)
	{
		String16 to;
		return try_convert(from, to) ? to : String16(default_value);
	}

	String16 to_utf16(StringView from, StringView16 default_value)
	{
		String16 to;
		return try_convert(from, to) ? to : String16(default_value);
	}

	StringW to_wcs(StringView32 from, StringViewW default_value)
	{
		StringW to;
		return try_convert(from, to) ? to : StringW(default_value);
	}

	StringW to_wcs(StringView16 from, StringViewW default_value)
	{
		StringW to;
		return try_convert(from, to) ? to : StringW(default_value);
	}

	StringW to_wcs(StringView from, StringViewW default_value)
	{
		StringW to;
		return try_convert(from, to) ? to : StringW(default_value);
	}

	String to_utf8(StringView32 from, StringView default_value)
	{
		String to;
		return try_convert(from, to) ? to : String(default_value);
	}

	String to_utf8(StringView16 from, StringView default_value)
	{
		String to;
		return try_convert(from, to) ? to : String(default_value);
	}

	String to_utf8(StringViewW from, StringView default_value)
	{
		String to;
		return try_convert(from, to) ? to : String(default_value);
	}
}