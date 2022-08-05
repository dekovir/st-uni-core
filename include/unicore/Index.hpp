#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename Type, typename Tag>
	struct Index
	{
		Type value;

		constexpr Index() = default;
		explicit constexpr Index(const Type& value_) noexcept : value(value_) {}

		template<typename... Args,
			std::enable_if_t<std::is_nothrow_constructible_v<Type, Args...>>* = nullptr>
		explicit constexpr Index(Args&&... args) noexcept : value(std::forward<Args>(args)...) {}

		Index& operator+(const Index& other)
		{
			value += other.value;
			return *this;
		}

		Index& operator-(const Index& other)
		{
			value -= other.value;
			return *this;
		}
	};

	template<typename Type, typename Tag>
	static constexpr bool operator==(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value == b.value;
	}

	template<typename Type, typename Tag>
	static constexpr bool operator!=(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return a.value != b.value;
	}

	template<typename Type, typename Tag>
	static constexpr Index<Type, Tag> operator+(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value + b.value);
	}

	template<typename Type, typename Tag>
	static constexpr Index<Type, Tag> operator-(const Index<Type, Tag>& a, const Index<Type, Tag>& b)
	{
		return Index<Type, Tag>(a.value - b.value);
	}
}