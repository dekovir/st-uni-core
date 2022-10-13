#pragma once
#include "unicore/math/Hash.hpp"

namespace unicore
{
	template <typename TChar, size_t N>
	struct ConstString
	{
		static constexpr size_t npos = static_cast<size_t>(-1);
		static constexpr size_t size = N;

		TChar data[N + 1];
		size_t hash;

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

		//UC_NODISCARD constexpr auto hash() const
		//{
		//	return Crc32::compute(data, N);
		//}

		template <size_t Pos, size_t Count = npos>
		UC_NODISCARD constexpr auto substr() const noexcept
		{
			static_assert(Pos <= N);
			constexpr size_t new_size = (std::min)(Count, N - Pos);

			TChar buf[new_size + 1]{};
			for (size_t i = 0; i < new_size; i++) {
				buf[i] = data[Pos + i];
			}

			return ConstString<TChar, new_size>(buf);
		}

		UC_NODISCARD constexpr auto find(TChar ch, size_t pos = 0) const noexcept
		{
			for (size_t i = pos; i < N; i++) {
				if (data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

		UC_NODISCARD constexpr auto rfind(TChar ch, size_t pos = npos) const noexcept
		{
			for (size_t i = (pos == npos ? N - 1 : pos); i + 1 > 0; i--) {
				if (data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

	private:
		template <size_t... Idx>
		constexpr ConstString(const ConstString<TChar, N>& other, std::index_sequence<Idx...>) noexcept
			: data{ other.data[Idx]... }, hash(Crc32::compute(view()))
		{
		}

		template <size_t... Idx>
		constexpr ConstString(const TChar(&data)[sizeof...(Idx) + 1], std::index_sequence<Idx...>) noexcept
			: data{ data[Idx]... }, hash(Crc32::compute(view()))
		{
		}
	};

	template <typename TChar>
	constexpr ConstString<TChar, 0> make_const_string() noexcept
	{
		return {};
	}

	template <typename TChar, size_t N>
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

	template <typename TChar, size_t N, size_t M>
	constexpr ConstString<TChar, N + M> operator+(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		TChar data[N + M + 1]{ };
		for (size_t i = 0; i < N; i++)
			data[i] = a.data[i];
		for (size_t i = 0; i < M; i++)
			data[N + i] = b.data[i];
		return data;
	}

	template <typename TChar, size_t N, size_t M>
	constexpr ConstString<TChar, N + M - 1> operator+(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a + make_const_string(b);
	}

	template <typename TChar, size_t N, size_t M>
	constexpr ConstString<TChar, N + M - 1> operator+(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) + b;
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator==(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		if constexpr (N != M) {
			return false;
		}

		for (size_t i = 0; i < M; i++) {
			if (a.data[i] != b.data[i]) {
				return false;
			}
		}

		return true;
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator!=(const ConstString<TChar, N>& a, const ConstString<TChar, M>& b) noexcept
	{
		return !(a == b);
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator==(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a == make_const_string(b);
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator!=(const ConstString<TChar, N>& a, const TChar(&b)[M]) noexcept
	{
		return a != make_const_string(b);
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator==(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) == b;
	}

	template <typename TChar, size_t N, size_t M>
	constexpr bool operator!=(const TChar(&a)[N], const ConstString<TChar, M>& b) noexcept
	{
		return make_const_string(a) != b;
	}

	namespace CharTable
	{
		static constexpr auto Punctuation = make_const_string(U"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
		static constexpr auto Digits = make_const_string(U"0123456789");
		static constexpr auto HexDigits = Digits + make_const_string(U"abcdefABCDEF");
		static constexpr auto Alpha = make_const_string(U"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
		static constexpr auto Alnum = Alpha + Digits;
		static constexpr auto Space = make_const_string(U" \f\n\r\t\v");

		static constexpr auto Ascii = Alpha + Punctuation + Digits;
		static constexpr auto Printable = Ascii + make_const_string(U" ");

		static constexpr auto English = Alpha;
		static constexpr auto Russian =
			make_const_string(U"АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя");

		// https://www.cogsci.ed.ac.uk/~richard/unicode-sample-3-2.html
	}
}