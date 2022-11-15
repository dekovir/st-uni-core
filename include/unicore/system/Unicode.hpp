#pragma once
#include "unicore/Defs.hpp"

namespace unicore::Unicode
{
	extern bool try_convert(StringView32 from, String16& to);
	extern bool try_convert(StringView32 from, StringW& to);
	extern bool try_convert(StringView32 from, String& to);

	extern bool try_convert(StringView16 from, String32& to);
	extern bool try_convert(StringView16 from, StringW& to);
	extern bool try_convert(StringView16 from, String& to);

	extern bool try_convert(StringViewW from, String32& to);
	extern bool try_convert(StringViewW from, String16& to);
	extern bool try_convert(StringViewW from, String& to);

	extern bool try_convert(StringView from, String32& to);
	extern bool try_convert(StringView from, String16& to);
	extern bool try_convert(StringView from, StringW& to);

	template<typename CharTo, typename CharFrom>
	extern Bool try_to(BasicStringView<CharFrom> from, BasicString<CharTo>& to)
	{
		if constexpr (std::is_same_v<CharTo, CharFrom>)
		{
			to = BasicString<CharTo>(from);
			return true;
		}

		return try_convert(from, to);
	}

	template<typename CharTo, typename CharFrom>
	extern BasicString<CharTo> to(BasicStringView<CharFrom> from, BasicStringView<CharTo> default_value = {})
	{
		BasicString<CharTo> to;
		return try_to(from, to) ? to : BasicString<CharTo>(default_value);
	}

	extern String32 to_utf32(StringView16 from, StringView32 default_value = U"");
	extern String32 to_utf32(StringViewW from, StringView32 default_value = U"");
	extern String32 to_utf32(StringView from, StringView32 default_value = U"");

	extern String16 to_utf16(StringView32 from, StringView16 default_value = u"");
	extern String16 to_utf16(StringViewW from, StringView16 default_value = u"");
	extern String16 to_utf16(StringView from, StringView16 default_value = u"");

	extern StringW to_wcs(StringView32 from, StringViewW default_value = L"");
	extern StringW to_wcs(StringView16 from, StringViewW default_value = L"");
	extern StringW to_wcs(StringView from, StringViewW default_value = L"");

	extern String to_utf8(StringView32 from, StringView default_value = "");
	extern String to_utf8(StringView16 from, StringView default_value = "");
	extern String to_utf8(StringViewW from, StringView default_value = "");
}