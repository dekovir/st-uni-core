#include "unicore/Type.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	Type::Type(StringView classname_, TypeConstPtr parent_, const std::type_info& info_) noexcept
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

	bool operator==(const Type& a, const Type& b)
	{
		return &a.info == &b.info;
	}

	bool operator!=(const Type& a, const Type& b)
	{
		return &a.info != &b.info;
	}

	bool operator<=(const Type& a, const Type& b)
	{
		return &a.info <= &b.info;
	}

	bool operator>=(const Type& a, const Type& b)
	{
		return &a.info >= &b.info;
	}

	bool operator<(const Type& a, const Type& b)
	{
		return &a.info < &b.info;
	}

	bool operator>(const Type& a, const Type& b)
	{
		return &a.info > &b.info;
	}

	LogHelper& operator<<(LogHelper& helper, const Type& type)
	{
		return helper << '<' << type.classname << '>';
	}
}