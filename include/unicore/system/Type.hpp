#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	struct Type;
	using TypeConstRef = const Type&;
	using TypeConstPtr = const Type*;

	template<typename T>
	static TypeConstRef get_type()
	{
		return T::type_static();
	}

	struct Type
	{
		StringView classname;
		TypeConstPtr parent;
		const TypeInfo& info;

		Type(StringView classname, TypeConstPtr parent, const TypeInfo& info) noexcept;

		UC_NODISCARD bool is_derived_from(TypeConstRef parent) const;

		template<typename T>
		UC_NODISCARD bool is_derived_from() const { return is_derived_from(get_type<T>()); }

		UC_NODISCARD bool can_cast_to(TypeConstRef type) const;

		template<typename T>
		UC_NODISCARD bool can_cast_to() const { return can_cast_to(get_type<T>()); }
	};

	static constexpr bool operator==(const Type& a, const Type& b);
	static constexpr bool operator!=(const Type& a, const Type& b);

	static constexpr bool operator<=(const Type& a, const Type& b);
	static constexpr bool operator>=(const Type& a, const Type& b);

	static constexpr bool operator<(const Type& a, const Type& b);
	static constexpr bool operator>(const Type& a, const Type& b);

	extern UNICODE_STRING_BUILDER_FORMAT(const Type&);
}