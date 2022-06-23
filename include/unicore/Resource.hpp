#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Resource : public virtual Object
	{
	public:
		UC_NODISCARD virtual size_t system_memory_use() const = 0;
	};
}