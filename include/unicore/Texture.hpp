#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/RendererResource.hpp"

namespace unicore
{
	class Texture : public RendererResource
	{
		UC_OBJECT(Texture, RendererResource)
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};
}