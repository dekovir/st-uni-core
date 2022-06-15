#include "unicore/Strings.hpp"
#include <codecvt>
#include <locale>

namespace unicore
{
	namespace Strings
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

		int compare(const StringView a, const StringView b, bool case_sensitive)
		{
			if (case_sensitive)
				return strcmp(a.data(), b.data());

#if defined(UNICORE_PLATFORM_WINDOWS)
			return _stricmp(a.data(), b.data());
#else
			return strcasecmp(a.data(), b.data());
#endif
		}

		int compare(const WStringView a, const WStringView b, bool case_sensitive)
		{
			if (case_sensitive)
				return wcscmp(a.data(), b.data());

#if defined(UNICORE_PLATFORM_WINDOWS)
			return _wcsicmp(a.data(), b.data());
#else 
			return wcscasecmp(a.data(), b.data());
#endif
		}
	}
}