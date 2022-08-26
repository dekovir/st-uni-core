#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Path.hpp"

namespace unicore
{
	class Texture;

	namespace CreateResource
	{
		template<typename T>
		using PathOrValue = Variant<Path, T>;

		struct TileSet
		{
			PathOrValue<Shared<Texture>> texture;
			Vector2i tile;
		};
	}

	// TODO: Do we need virtual methods?
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
	using SpriteAtlas = ResourceAtlas<Sprite>;
}