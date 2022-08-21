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

	class DynamicTexture : public Texture
	{
		UC_OBJECT(DynamicTexture, Texture)
	public:
	};

	class TargetTexture : public Texture
	{
		UC_OBJECT(TargetTexture, Texture)
	public:
	};
}