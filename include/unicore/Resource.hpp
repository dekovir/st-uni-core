#pragma once
#include <any>
#include "unicore/Object.hpp"

namespace unicore
{
	enum class ResourceCachePolicy : uint8_t
	{
		CanCache,
		NoCache,
	};

	class Resource : public virtual Object
	{
		UC_OBJECT(Resource, Object)
	public:
		UC_NODISCARD virtual size_t get_system_memory_use() const = 0;

		UC_NODISCARD virtual size_t get_used_resources(Set<Shared<Resource>>& resources) { return 0; }
		UC_NODISCARD virtual ResourceCachePolicy cache_policy() const { return ResourceCachePolicy::CanCache; }
	};
}