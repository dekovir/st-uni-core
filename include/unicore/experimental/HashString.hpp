#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class HashString
	{
	public:
		constexpr explicit HashString(size_t value)
			: _value(value) {}

		UC_NODISCARD constexpr size_t value() const { return _value; }

		static constexpr HashString calc(StringView data)
		{
			return HashString(0);
		}

	protected:
		size_t _value;
	};

	static constexpr bool operator==(const HashString& a, const HashString& b)
	{
		return a.value() == b.value();
	}

	static constexpr bool operator!=(const HashString& a, const HashString& b)
	{
		return a.value() != b.value();
	}

	static constexpr bool operator<=(const HashString& a, const HashString& b)
	{
		return a.value() <= b.value();
	}

	static constexpr bool operator>=(const HashString& a, const HashString& b)
	{
		return a.value() >= b.value();
	}

	static constexpr bool operator<(const HashString& a, const HashString& b)
	{
		return a.value() < b.value();
	}

	static constexpr bool operator>(const HashString& a, const HashString& b)
	{
		return a.value() > b.value();
	}

	static constexpr HashString operator"" _hash(const char* path, size_t len)
	{
		return HashString::calc(StringView(path, len));
	}
}