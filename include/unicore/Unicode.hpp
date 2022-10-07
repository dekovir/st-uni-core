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
	static BasicString<CharTo> to(BasicStringView<CharFrom> from, bool* success = nullptr)
	{
		if constexpr (std::is_same_v<CharTo, CharFrom>)
		{
			if (success != nullptr) *success = true;
			return from;
		}

		BasicString<CharTo> result;
		if (try_convert(from, result))
		{
			if (success != nullptr) *success = true;
			return result;
		}

		if (success != nullptr) *success = false;
		return {};
	}

	extern String32 to_utf32(StringView16 from, bool* success = nullptr);
	extern String32 to_utf32(StringViewW from, bool* success = nullptr);
	extern String32 to_utf32(StringView from, bool* success = nullptr);

	extern String16 to_utf16(StringView32 from, bool* success = nullptr);
	extern String16 to_utf16(StringViewW from, bool* success = nullptr);
	extern String16 to_utf16(StringView from, bool* success = nullptr);

	extern StringW to_wcs(StringView32 from, bool* success = nullptr);
	extern StringW to_wcs(StringView16 from, bool* success = nullptr);
	extern StringW to_wcs(StringView from, bool* success = nullptr);

	extern String to_utf8(StringView32 from, bool* success = nullptr);
	extern String to_utf8(StringView16 from, bool* success = nullptr);
	extern String to_utf8(StringViewW from, bool* success = nullptr);
}