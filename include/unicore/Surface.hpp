#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/RenderObject.hpp"

namespace unicore
{
	class Surface : public RenderObject
	{
	public:
		virtual const Vector2i& size() const = 0;
	};
}
