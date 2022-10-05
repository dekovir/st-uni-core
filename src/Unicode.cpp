#include "unicore/Unicode.hpp"
#include <codecvt>
#include <locale>

namespace unicore::Unicode
{
	bool try_convert(const WStringView from, String& to)
	{
		// TODO: Replace with no exception library
		//try
		//{
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
		to = conv.to_bytes(from.data());
		return true;
		//}
		//catch (...)
		//{
		//	return false;
		//}
	}

	bool try_convert(const std::basic_string_view<char16_t> from, String& to)
	{
		static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
		to = converter.to_bytes(from.data());
		return true;
	}

	bool try_convert(const StringView from, WString& to)
	{
		// TODO: Replace with no exception library
		//try
		//{
		static std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > conv;
		to = conv.from_bytes(from.data());
		return true;
		//}
		//catch (...)
		//{
		//	return false;
		//}
	}

	//
	String to_utf8(const WStringView str, bool* success)
	{
		std::string result;
		if (try_convert(str, result))
		{
			if (success != nullptr) *success = true;
			return result;
		}

		if (success != nullptr) *success = false;
		return {};
	}

	String to_utf8(const BasicStringView<char16_t> str, bool* success)
	{
		std::string result;
		if (try_convert(str, result))
		{
			if (success != nullptr) *success = true;
			return result;
		}

		if (success != nullptr) *success = false;
		return {};
	}

	WString to_wcs(const StringView str, bool* success)
	{
		std::wstring result;
		if (try_convert(str, result))
		{
			if (success != nullptr) *success = true;
			return result;
		}

		if (success != nullptr) *success = false;
		return {};
	}

	WString to_wcs(const BasicStringView<char16_t> str, bool* success)
	{
		// TODO: Optimize convert
		return to_wcs(to_utf8(str), success);
	}
}
