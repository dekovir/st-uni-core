#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template <size_t N>
	struct ConstString
	{
		static constexpr size_t npos = static_cast<size_t>(-1);
		static constexpr size_t size = N;

		char data[N + 1];

		constexpr ConstString() noexcept
			: data{}
		{
		}

		constexpr ConstString(const ConstString<N>& other) noexcept
			: ConstString(other, std::make_index_sequence<N>())
		{
		}

		constexpr ConstString(const char(&data)[N + 1]) noexcept
			: ConstString(data, std::make_index_sequence<N>())
		{
		}

		explicit constexpr operator const char* () const noexcept
		{
			return data;
		}

		explicit operator std::string() const noexcept
		{
			return data;
		}

		UC_NODISCARD constexpr const char* c_str() const noexcept
		{
			return data;
		}

		UC_NODISCARD std::string str() const noexcept
		{
			return data;
		}

		template <size_t Pos, size_t Count = npos>
		UC_NODISCARD constexpr auto substr() const noexcept
		{
			static_assert(Pos <= N);
			constexpr size_t new_size = (std::min)(Count, N - Pos);

			char buf[new_size + 1]{};
			for (size_t i = 0; i < new_size; i++) {
				buf[i] = data[Pos + i];
			}

			return ConstString<new_size>(buf);
		}

		UC_NODISCARD constexpr auto find(char ch, size_t pos = 0) const noexcept
		{
			for (size_t i = pos; i < N; i++) {
				if (data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

		UC_NODISCARD constexpr auto rfind(char ch, size_t pos = npos) const noexcept
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
		constexpr ConstString(const ConstString<N>& other, std::index_sequence<Idx...>) noexcept
			: data{ other.data[Idx]... }
		{
		}

		template <size_t... Idx>
		constexpr ConstString(const char(&data)[sizeof...(Idx) + 1], std::index_sequence<Idx...>) noexcept
			: data{ data[Idx]... }
		{
		}
	};

	constexpr ConstString<0> make_const_string() noexcept
	{
		return {};
	}

	template <size_t N>
	constexpr ConstString<N - 1> make_const_string(const char(&str)[N]) noexcept
	{
		return str;
	}

	constexpr ConstString<1> make_const_string(char ch) noexcept
	{
		const char str[2]{ ch, '\0' };
		return make_const_string(str);
	}

	template <size_t N, size_t M>
	constexpr ConstString<N + M> operator+(const ConstString<N>& a, const ConstString<M>& b) noexcept
	{
		char data[N + M + 1]{ };
		for (size_t i = 0; i < N; i++)
			data[i] = a.data[i];
		for (size_t i = 0; i < M; i++)
			data[N + i] = b.data[i];
		return data;
	}

	template <size_t N, size_t M>
	constexpr ConstString<N + M - 1> operator+(const ConstString<N>& a, const char(&b)[M]) noexcept
	{
		return a + make_const_string(b);
	}

	template <size_t N, size_t M>
	constexpr ConstString<N + M - 1> operator+(const char(&a)[N], const ConstString<M>& b) noexcept
	{
		return make_const_string(a) + b;
	}

	template <size_t N, size_t M>
	constexpr bool operator==(const ConstString<N>& a, const ConstString<M>& b) noexcept
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

	template <size_t N, size_t M>
	constexpr bool operator!=(const ConstString<N>& a, const ConstString<M>& b) noexcept
	{
		return !(a == b);
	}

	template <size_t N, size_t M>
	constexpr bool operator==(const ConstString<N>& a, const char(&b)[M]) noexcept
	{
		return a == make_const_string(b);
	}

	template <size_t N, size_t M>
	constexpr bool operator!=(const ConstString<N>& a, const char(&b)[M]) noexcept
	{
		return a != make_const_string(b);
	}

	template <size_t N, size_t M>
	constexpr bool operator==(const char(&a)[N], const ConstString<M>& b) noexcept
	{
		return make_const_string(a) == b;
	}

	template <size_t N, size_t M>
	constexpr bool operator!=(const char(&a)[N], const ConstString<M>& b) noexcept
	{
		return make_const_string(a) != b;
	}

	template <size_t N>
	extern UNICODE_STRING_BUILDER_FORMAT(const ConstString<N>&)
	{
		return builder << value.c_str();
	}
}