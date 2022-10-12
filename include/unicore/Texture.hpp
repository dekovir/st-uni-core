#pragma once
#include "unicore/RendererResource.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	// TODO: on_resize event for derived classed and reload?
	class Texture : public RendererResource
	{
		UC_OBJECT(Texture, RendererResource)
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};

	using TextureList = ResourceList<Texture>;
	using TextureDictionary = ResourceDictionary<Texture>;

	class DynamicTexture : public Texture
	{
		UC_OBJECT(DynamicTexture, Texture)
	public:
		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }
	};

	class TargetTexture : public Texture
	{
		UC_OBJECT(TargetTexture, Texture)
	public:
		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }
	};
}