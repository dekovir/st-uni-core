#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename Type, typename Tag>
	struct Index
	{
		Type value;

		constexpr Index() = default;
		explicit constexpr Index(const Type& value_) : value(value_) {}
	};

	template<typename Type, typename Tag>
	static constexpr bool operator==(const Index<Type,Tag>& a, const Index<Type,Tag>& b)
	{
		return a.value == b.value;
	}

	template<typename Type, typename Tag>
	static constexpr bool operator!=(const Index<Type,Tag>& a, const Index<Type,Tag>& b)
	{
		return a.value != b.value;
	}
}