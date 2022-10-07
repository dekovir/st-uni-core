#include "unicore/Unicode.hpp"
#include <codecvt>
#include <locale>

namespace unicore::Unicode
{
	bool try_convert(StringView32 from, String16& to)
	{
		std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
		std::string bytes = conv.to_bytes(from.data());
		to = std::u16string(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length() / sizeof(char16_t));
		return true;
	}

	bool try_convert(StringView32 from, std::wstring& to)
	{
		String utf8;
		return
			try_convert(from, utf8) &&
			try_convert(utf8, to);
	}

	bool try_convert(StringView32 from, String& to)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		to = conv.to_bytes(from.data());
		return true;
	}

	bool try_convert(StringView16 from, String32& to)
	{
		std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
		to = conv.from_bytes(reinterpret_cast<const char*>(from.data()), reinterpret_cast<const char*>(from.data() + from.size()));
		return true;
	}

	bool try_convert(StringView16 from, std::wstring& to)
	{
		String str;
		return
			try_convert(from, str) &&
			try_convert(str, to);
	}

	bool try_convert(StringView16 from, String& to)
	{
		std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
		to = conv.to_bytes(from.data());
		return true;
	}

	bool try_convert(std::wstring_view from, String32& to)
	{
		String str;
		return
			try_convert(from, str) &&
			try_convert(str, to);
	}

	bool try_convert(std::wstring_view from, String16& to)
	{
		String str;
		return
			try_convert(from, str) &&
			try_convert(str, to);
	}

	bool try_convert(std::wstring_view from, String& to)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
		to = conv.to_bytes(from.data());
		return true;
	}

	bool try_convert(StringView from, String32& to)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		to = conv.from_bytes(from.data());
		return true;
	}

	bool try_convert(StringView from, String16& to)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
		to = conv.from_bytes(from.data());
		return true;
	}

	bool try_convert(StringView from, std::wstring& to)
	{
		static std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > conv;
		to = conv.from_bytes(from.data());
		return true;
	}

	String32 to_utf32(StringView from, bool* success)
	{
		return to<Char32>(from, success);
	}

	String32 to_utf32(StringView16 from, bool* success)
	{
		return to<Char32>(from, success);
	}

	String32 to_utf32(std::wstring_view from, bool* success)
	{
		return to<Char32>(from, success);
	}

	String16 to_utf16(StringView from, bool* success)
	{
		return to<Char16>(from, success);
	}

	String16 to_utf16(StringView32 from, bool* success)
	{
		return to<Char16>(from, success);
	}

	std::wstring to_wcs(StringView from, bool* success)
	{
		return to<wchar_t>(from, success);
	}

	std::wstring to_wcs(StringView32 from, bool* success)
	{
		return to<wchar_t>(from, success);
	}

	String to_utf8(StringView32 from, bool* success)
	{
		return to<Char>(from, success);
	}

	String to_utf8(StringView16 from, bool* success)
	{
		return to<Char>(from, success);
	}

	String to_utf8(std::wstring_view from, bool* success)
	{
		return to<Char>(from, success);
	}
}