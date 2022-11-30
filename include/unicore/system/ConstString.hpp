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
}