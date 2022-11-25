#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/ui/schemes/SchemeNode.hpp"

namespace unicore::ui
{
	class Scheme : public Resource
	{
		UC_OBJECT(Scheme, Resource)
	public:
		UC_NODISCARD Size get_system_memory_use() const override;
	};
}