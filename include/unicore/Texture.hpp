#pragma once
#include "unicore/RendererResource.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	enum class TextureType
	{
		Static,
		Dynamic,
		Target,
	};

	// TODO: on_resize event for derived classed and reload?
	class Texture : public RendererResource
	{
		UC_OBJECT(Texture, RendererResource)
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
		UC_NODISCARD virtual TextureType texture_type() const { return TextureType::Static; }
	};

	using TextureList = ResourceList<Texture>;
	using TextureDictionary = ResourceDictionary<Texture>;

	//TODO: Remove?
	class DynamicTexture : public Texture
	{
		UC_OBJECT(DynamicTexture, Texture)
	public:
		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }
		UC_NODISCARD TextureType texture_type() const override { return TextureType::Dynamic; }
	};

	//TODO: Remove?
	class TargetTexture : public Texture
	{
		UC_OBJECT(TargetTexture, Texture)
	public:
		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }
		UC_NODISCARD TextureType texture_type() const override { return TextureType::Target; }
	};
}