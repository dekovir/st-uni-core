#pragma once
#include "unicore/Defs.hpp"

namespace unicore::Unicode
{
	extern bool try_convert(const WStringView from, String& to);
	extern bool try_convert(const std::basic_string_view<char16_t> from, String& to);

	extern bool try_convert(const StringView from, WString& to);

	extern String to_utf8(const WStringView str, bool* success = nullptr);
	extern String to_utf8(const std::basic_string_view<char16_t> str, bool* success = nullptr);

	extern WString to_wcs(const StringView str, bool* success = nullptr);
}