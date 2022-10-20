#include "unicore/renderer/SpriteListTileSetLoader.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/renderer/Texture.hpp"

namespace unicore
{
	Shared<Resource> SpriteListTileSetLoader::load_options(
		const Context& context, const TileSetOptions& options)
	{
		const auto tex = context.cache.load<Texture>(context.path);
		if (!tex) return nullptr;

		const auto size = tex->size();
		if (size.x < options.tile.x || size.y < options.tile.y)
			return nullptr;

		const auto count_x = std::div(size.x, options.tile.x).quot;
		const auto count_y = std::div(size.y, options.tile.y).quot;

		SpriteList::DataType list;
		for (int i_y = 0; i_y < count_y; i_y++)
			for (int i_x = 0; i_x < count_x; i_x++)
			{
				const Vector2i pos(i_x * options.tile.x, i_y * options.tile.y);
				auto spr = std::make_shared<Sprite>(
					tex, Recti(pos, options.tile));
				list.push_back(spr);
			}

		return std::make_shared<SpriteList>(std::move(list));
	}
}