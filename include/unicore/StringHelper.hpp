#pragma once
#include "unicore/Defs.hpp"

namespace unicore::StringHelper
{
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
	extern int compare(const StringView a, const StringView b, bool case_sensitive);
	extern int compare(const WStringView a, const WStringView b, bool case_sensitive);

	extern String to_hex(intptr_t value);

	extern String print_format(StringView format, ...);

	template<typename Char>
	static bool starts_with(const BasicStringView<Char> string,
		const BasicStringView<Char> prefix, bool case_sensitive)
	{
		if (string.length() >= prefix.length())
		{
			return compare(
				string.data(), prefix.data(), case_sensitive) == 0;
		}

		return false;
	}

	template<typename Char>
	static bool ends_with(const BasicStringView<Char> string,
		const BasicStringView<Char> ending, bool case_sensitive)
	{
		if (string.length() >= ending.length())
		{
			const auto offset = string.length() - ending.length();
			return compare(
				string.data() + offset, ending.data(), case_sensitive) == 0;
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
	extern bool compare_to_mask(BasicStringView<TChar> string_, BasicStringView<TChar> wild_)
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