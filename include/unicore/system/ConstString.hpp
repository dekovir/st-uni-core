#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template <typename TChar, Size N>
	struct ConstString
	{
		static constexpr Size npos = BasicStringView<TChar>::npos;
		static constexpr Size size = N;

		TChar data[N + 1];

		constexpr ConstString() noexcept
			: data{}
		{
		}

		constexpr ConstString(const ConstString<TChar, N>& other) noexcept
			: ConstString(other, std::make_index_sequence<N>())
		{
		}

		constexpr ConstString(const TChar(&data)[N + 1]) noexcept
			: ConstString(data, std::make_index_sequence<N>())
		{
		}

		constexpr TChar operator[](Size index) const noexcept
		{
			return data[index];
		}

		explicit constexpr operator const TChar* () const noexcept
		{
			return data;
		}

		explicit operator BasicStringView<TChar>() const noexcept
		{
			return view();
		}

		UC_NODISCARD constexpr const TChar* c_str() const noexcept
		{
			return data;
		}

		UC_NODISCARD constexpr BasicStringView<TChar> view() const noexcept
		{
			return BasicStringView<TChar>(data, N + 1);
		}

		UC_NODISCARD constexpr BasicString<TChar> str() const noexcept
		{
			return data;
		}

		template <Size Pos, Size Count = npos>
		UC_NODISCARD constexpr auto substr() const noexcept
		{
			static_assert(Pos <= N);
			constexpr Size new_size = std::min(Count, N - Pos);

			TChar buf[new_size + 1]{};
			for (Size i = 0; i < new_size; i++) {
				buf[i] = data[Pos + i];
			}

			return ConstString<TChar, new_size>(buf);
		}

		UC_NODISCARD constexpr auto find(TChar ch, Size pos = 0) const noexcept
		{
			for (Size i = pos; i < N; i++) {
				if (data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

		UC_NODISCARD constexpr auto rfind(TChar ch, Size pos = npos) const noexcept
		{
			for (Size i = (pos == npos ? N - 1 : pos); i + 1 > 0; --i) {
				if (data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

	private:
		template <Size... Idx>
		constexpr ConstString(const ConstString<TChar, N>& other, std::index_sequence<Idx...>) noexcept
			: data{ other.data[Idx]... }
		{
		}

		template <Size... Idx>
		constexpr ConstString(const TChar(&data)[sizeof...(Idx) + 1], std::index_sequence<Idx...>) noexcept
			: data{ data[Idx]... }
		{
		}
	};

	template <typename TChar>
	constexpr ConstString<TChar, 0> make_const_string() noexcept
	{
		return {};
	}

	template <typename TChar, Size N>
	constexpr ConstString<TChar, N - 1> make_const_string(const TChar(&str)[N]) noexcept
	{
		return str;
	}

	template <typename TChar>
	constexpr ConstString<TChar, 1> make_const_string(TChar ch) noexcept
	{
		const TChar str[2]{ ch, '\0' };
		return make_const_string(str);
	}

	template <typename TChar, Size N, Size M>
	constexpr ConstString<TChar, N + M> operator+(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		TChar data[N + M + 1]{ };
		for (Size i = 0; i < N; i++)
			data[i] = a.data[i];
		for (Size i = 0; i < M; i++)
			data[N + i] = b.data[i];
		return data;
	}

	template <typename TChar, Size N, Size M>
	constexpr ConstString<TChar, N + M - 1> operator+(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a + make_const_string(b);
	}

	template <typename TChar, Size N, Size M>
	constexpr ConstString<TChar, N + M - 1> operator+(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) + b;
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator==(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		if constexpr (N != M) {
			return false;
		}

		for (Size i = 0; i < M; i++) {
			if (a.data[i] != b.data[i]) {
				return false;
			}
		}

		return true;
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator!=(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		return !(a == b);
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator==(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a == make_const_string(b);
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator!=(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a != make_const_string(b);
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator==(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) == b;
	}

	template <typename TChar, Size N, Size M>
	constexpr bool operator!=(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) != b;
	}

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