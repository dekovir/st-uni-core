#include "unicore/Type.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	Type::Type(StringView classname_, TypeConstPtr parent_, const TypeInfo& info_) noexcept
		: classname(classname_)
		, parent(parent_)
		, info(info_)
	{
	}

	bool Type::is_derived_from(TypeConstRef parent) const
	{
		for (auto type = this; type != nullptr; type = type->parent)
		{
			if (type == &parent)
				return true;
		}
		return false;
	}

	constexpr bool operator==(const Type& a, const Type& b)
	{
		return &a.info == &b.info;
	}

	constexpr bool operator!=(const Type& a, const Type& b)
	{
		return &a.info != &b.info;
	}

	constexpr bool operator<=(const Type& a, const Type& b)
	{
		return &a.info <= &b.info;
	}

	constexpr bool operator>=(const Type& a, const Type& b)
	{
		return &a.info >= &b.info;
	}

	constexpr bool operator<(const Type& a, const Type& b)
	{
		return &a.info < &b.info;
	}

	constexpr bool operator>(const Type& a, const Type& b)
	{
		return &a.info > &b.info;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Type&)
	{
		return builder << '<' << value.classname << '>';
	}
}