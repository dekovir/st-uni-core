#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename Type, typename Tag>
	struct Index
	{
		using TypeValue = Type;

		Type value;

		constexpr Index() = default;
		explicit constexpr Index(const Type& value_) noexcept : value(value_) {}

		template<typename... Args,
			std::enable_if_t<std::is_nothrow_constructible_v<Type, Args...>>* = nullptr>
		explicit constexpr Index(Args&&... args) noexcept : value(std::forward<Args>(args)...) {}
	};

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() == std::declval<Type>())>>* = nullptr>
	static constexpr bool operator==(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value == b.value;
	}

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() != std::declval<Type>())>>* = nullptr>
	static constexpr bool operator!=(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value != b.value;
	}

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() <= std::declval<Type>())>>* = nullptr>
	static constexpr bool operator<=(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value <= b.value;
	}

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() >= std::declval<Type>())>>* = nullptr>
	static constexpr bool operator>=(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value >= b.value;
	}

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() < std::declval<Type>())>>* = nullptr>
		static constexpr bool operator<(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value < b.value;
	}

	template<typename Type, typename Tag,
		std::enable_if_t<std::is_same_v<bool, decltype(std::declval<Type>() > std::declval<Type>())>>* = nullptr>
		static constexpr bool operator>(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value > b.value;
	}

	template<typename Type, typename Tag,
		class = decltype(std::declval<Type>() + std::declval<Type>())>
	static constexpr Index<Type, Tag> operator+(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value + b.value);
	}

	template<typename Type, typename Tag,
		class = decltype(std::declval<Type>() - std::declval<Type>())>
	static constexpr Index<Type, Tag> operator-(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value - b.value);
	}

	template<typename Type, typename Tag,
		class = decltype(std::declval<Type>()* std::declval<Type>())>
	static constexpr Index<Type, Tag> operator*(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value * b.value);
	}

	template<typename Type, typename Tag,
		class = decltype(std::declval<Type>() / std::declval<Type>())>
	static constexpr Index<Type, Tag> operator/(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value / b.value);
	}
}

#define UNICORE_MAKE_INDEX(name, type) \
	namespace details::tag { struct name {}; } \
	using name = Index<type, details::tag::name>;

#define UNICORE_MAKE_INDEX_WITH_INVALID(name, type) \
	UNICORE_MAKE_INDEX(name, type) \
	static constexpr name name ## _Invalid = name(std::numeric_limits<name::TypeValue>::max())