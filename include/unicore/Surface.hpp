#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class Surface : public Resource
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};
}
