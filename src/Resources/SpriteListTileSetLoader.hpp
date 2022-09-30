#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Sprite.hpp"

namespace unicore
{
	class SpriteListTileSetLoader : public ResourceLoaderOptionsTyped<
		SpriteList, TileSetOptions, ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(SpriteListTileSetLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const TileSetOptions& options) override;
	};
}