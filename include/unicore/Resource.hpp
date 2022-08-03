#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Resource : public virtual Object
	{
		UC_OBJECT(Resource, Object)
	public:
		UC_NODISCARD virtual size_t get_system_memory_use() const = 0;
		virtual size_t get_used_resources(Set<Shared<Resource>>& resources) { return 0; }
	};
}