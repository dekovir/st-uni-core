#pragma once
#include "unicore/system/ConstString.hpp"

namespace unicore
{
	namespace AsciiTable
	{
		static constexpr auto AlphabetUpper = make_const_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		static constexpr auto AlphabetLower = make_const_string("abcdefghijklmnopqrstuvwxyz");
		static constexpr auto Alphabet = AlphabetUpper + AlphabetLower;

		static constexpr auto Punctuation = make_const_string("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
		static constexpr auto Digits = make_const_string("0123456789");
		static constexpr auto HexDigits = Digits + make_const_string("abcdefABCDEF");

		static constexpr auto Space = make_const_string(" ");
		static constexpr auto Whitespace = Space + make_const_string(" \f\n\r\t\v");

		static constexpr auto All = Alphabet + Punctuation + Digits + Space;

		extern constexpr Char to_lower(Char c)
		{
			const auto pos = AlphabetUpper.find(c);
			return pos != ConstString<Char, 0>::npos ? AlphabetLower[pos] : c;
		}

		extern constexpr Char to_upper(Char c)
		{
			const auto pos = AlphabetLower.find(c);
			return pos != ConstString<Char, 0>::npos ? AlphabetUpper[pos] : c;
		}

		template <Size N>
		extern constexpr ConstString<Char, N> to_lower(const ConstString<Char, N>& str)
		{
			Char data[N + 1]{ };
			for (Size i = 0; i < N; i++)
				data[i] = to_lower(str[i]);
			return data;
		}

		template <Size N>
		extern constexpr ConstString<Char, N> to_upper(const ConstString<Char, N>& str)
		{
			Char data[N + 1]{ };
			for (Size i = 0; i < N; i++)
				data[i] = to_upper(str[i]);
			return data;
		}
	}
}