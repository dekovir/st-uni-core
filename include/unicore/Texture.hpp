#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/RenderObject.hpp"

namespace unicore
{
	class Texture : public RenderObject
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};
}
