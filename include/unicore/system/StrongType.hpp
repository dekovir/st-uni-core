#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template <typename T, typename Parameter>
	class StrongType
	{
	public:
		StrongType() : _value({}) {}
		explicit StrongType(T const& value) : _value(value) {}
		explicit StrongType(T&& value) : _value(std::move(value)) {}

		T& get() { return _value; }
		UC_NODISCARD T const& get() const { return _value; }

	private:
		T _value;
	};

#define UNICORE_MAKE_STRONG_TYPE(name, type) \
	namespace details::tag { struct name {}; } \
	using name = StrongType<type, details::tag::name>;
}