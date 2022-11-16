#pragma once
#include "unicore/system/ConstString.hpp"

namespace unicore
{
	namespace UnicodeTable
	{
		static constexpr auto EnglishUpper =
			make_const_string(U"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		static constexpr auto EnglishLower =
			make_const_string(U"abcdefghijklmnopqrstuvwxyz");
		static constexpr auto English = EnglishUpper + EnglishLower;

		static constexpr auto RussianUpper =
			make_const_string(U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
		static constexpr auto RussianLower =
			make_const_string(U"абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
		static constexpr auto Russian = RussianUpper + RussianLower;

		static constexpr auto AllUpper = EnglishUpper + RussianUpper;
		static constexpr auto AllLower = EnglishLower + RussianLower;

		static constexpr auto Punctuation = make_const_string(U"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
		static constexpr auto Digits = make_const_string(U"0123456789");

		static constexpr auto Space = make_const_string(U" ");
		static constexpr auto Whitespace = Space + make_const_string(U" \f\n\r\t\v");

		static constexpr auto Currency = make_const_string(U"₠₡₢₣₤₥₦₧₨₩₪₫€₭₮₯₰₱");

		static constexpr auto Ascii = English + Punctuation + Digits + Space;

		// https://www.cogsci.ed.ac.uk/~richard/unicode-sample-3-2.html

		extern constexpr Char32 to_lower(Char32 c)
		{
			const auto pos = AllUpper.find(c);
			return pos != ConstString<Char32, 0>::npos ? AllLower[pos] : c;
		}

		extern constexpr Char32 to_upper(Char32 c)
		{
			const auto pos = AllLower.find(c);
			return pos != ConstString<Char32, 0>::npos ? AllUpper[pos] : c;
		}

		template <Size N>
		extern constexpr ConstString<Char32, N> to_lower(const ConstString<Char32, N>& str)
		{
			Char32 data[N + 1]{ };
			for (Size i = 0; i < N; i++)
				data[i] = to_lower(str[i]);
			return data;
		}

		template <Size N>
		extern constexpr ConstString<Char32, N> to_upper(const ConstString<Char32, N>& str)
		{
			Char32 data[N + 1]{ };
			for (Size i = 0; i < N; i++)
				data[i] = to_upper(str[i]);
			return data;
		}
	}
}