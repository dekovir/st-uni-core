#pragma once
#include "unicore/Object.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Display : public Object
	{
		UC_OBJECT(Display, Object)
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
		UC_NODISCARD virtual void* handle() const = 0;
	};
}