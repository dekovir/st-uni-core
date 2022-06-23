#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/RenderResource.hpp"

namespace unicore
{
	class Texture : public RenderResource
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};
}
