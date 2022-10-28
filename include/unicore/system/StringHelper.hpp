#pragma once
#include "unicore/Defs.hpp"

namespace unicore::StringHelper
{
	namespace sfinae
	{
		template <class T>
		constexpr bool is_char_v = std::is_same_v<T, char> || std::is_same_v<T, Char16> || std::is_same_v<T, Char32>;
	}

	template<typename TChar, typename Tag>
	struct Data
	{
		static BasicString<TChar> string;
	};

	template<typename TChar, typename Tag>
	BasicString<TChar> Data<TChar, Tag>::string;

	struct DefaultTag {};
	using DefaultString = Data<char, DefaultTag>;
	using DefaultWString = Data<wchar_t, DefaultTag>;

	// METHODS ////////////////////////////////////////////////////////////////////
	extern String to_hex(intptr_t value);
	extern String print_format(StringView format, ...);
	extern String to_lower(StringView str);

	extern Int compare(StringView a, StringView b, bool case_insensetive = false);
	extern Bool equals(StringView a, StringView b, bool case_insensetive = false);

	template<typename Char>
	extern BasicString<Char> ltrim(BasicStringView<Char> string,
		BasicStringView<Char> chars = " \t\n\r\f\v")
	{
		BasicString<Char> s(string);
		s.erase(0, s.find_first_not_of(chars));
		return s;
	}

	template<typename Char>
	extern BasicString<Char> rtrim(BasicStringView<Char> string,
		BasicStringView<Char> chars = " \t\n\r\f\v")
	{
		BasicString<Char> s(string);
		s.erase(s.find_last_not_of(chars) + 1);
		return s;
	}

	template<typename Char>
	extern BasicString<Char> trim(BasicStringView<Char> string,
		BasicStringView<Char> chars = " \t\n\r\f\v")
	{
		BasicString<Char> s(string);
		s.erase(0, s.find_first_not_of(chars));
		s.erase(s.find_last_not_of(chars) + 1);
		return s;
	}

	template<typename Char>
	extern Bool starts_with(const BasicStringView<Char> string,
		const BasicStringView<Char> prefix)
	{
		if (string.length() >= prefix.length())
			return string.substr(0, prefix.length()) == prefix;

		return false;
	}

	template<typename Char>
	extern Bool ends_with(const BasicStringView<Char> string,
		const BasicStringView<Char> ending)
	{
		if (string.length() >= ending.length())
		{
			const auto offset = string.length() - ending.length();
			return BasicStringView<Char>(string.data() + offset).compare(ending) == 0;
		}

		return false;
	}

	template<typename TChar>
	struct SpecialChars
	{
	};

	template<>
	struct SpecialChars<char>
	{
		static constexpr char AsteriskChar = '*';
		static constexpr char QuestionMark = '?';
	};

	template<>
	struct SpecialChars<wchar_t>
	{
		static constexpr wchar_t AsteriskChar = L'*';
		static constexpr wchar_t QuestionMark = L'?';
	};

	template<typename TChar>
	extern Bool compare_to_mask(BasicStringView<TChar> string_, BasicStringView<TChar> wild_)
	{
		auto string = string_.data();
		auto wild = wild_.data();
		const TChar* cp = nullptr, * mp = nullptr;

		constexpr auto AsteriskChar = SpecialChars<TChar>::AsteriskChar;
		constexpr auto QuestionMark = SpecialChars<TChar>::QuestionMark;

		while ((*string) && (*wild != AsteriskChar))
		{
			if ((*wild != *string) && (*wild != QuestionMark))
				return false;

			++wild;
			++string;
		}

		while (*string)
		{
			if (*wild == AsteriskChar)
			{
				if (!*++wild)
					return true;

				mp = wild;
				cp = string + 1;
			}
			else if ((*wild == *string) || (*wild == QuestionMark))
			{
				++wild;
				++string;
			}
			else
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == AsteriskChar)
			++wild;

		return !*wild;
	}
}