#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/math/Rect.hpp"

namespace unicore
{
	class Texture;

	class TileSetOptions : public ResourceOptions
	{
	public:
		Vector2i tile = Vector2i(32, 32);

		TileSetOptions() = default;
		explicit TileSetOptions(const Vector2i& tile_) : tile(tile_) {}

		UC_NODISCARD size_t hash() const override { return Hash::make(tile); }
	};

	class Sprite : public Resource
	{
		UC_OBJECT(Sprite, Resource)
	public:
		explicit Sprite(const Shared<Texture>& texture);
		Sprite(const Shared<Texture>& texture, const Recti& rect);

		UC_NODISCARD const Recti& rect() const { return _rect; }
		UC_NODISCARD const Shared<Texture>& texture() const { return _texture; }

		UC_NODISCARD size_t get_system_memory_use() const override;
		size_t get_used_resources(Set<Shared<Resource>>& resources) override;

	protected:
		Shared<Texture> _texture;
		Recti _rect;
	};

	using SpriteList = ResourceList<Sprite>;
	using SpriteDictionary = ResourceDictionary<Sprite>;
}