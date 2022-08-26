#include "SpriteListTileSetCreator.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	static Shared<T> load_resource(
		const ResourceCreatorContext& context,
		const CreateResource::PathOrValue<Shared<T>>& value)
	{
		if (auto path = std::get_if<Path>(&value))
			return context.cache.load<T>(*path);

		if (auto ptr = std::get_if<Shared<T>>(&value))
			return *ptr;

		return nullptr;
	}

	Shared<SpriteList> SpriteListTileSetCreator::create_from_data(const ResourceCreatorContext& context,
		const CreateResource::TileSet& data)
	{
		auto tex = load_resource(context, data.texture);
		//auto tex = context.cache.load<Texture>(data.path);
		if (!tex) return nullptr;

		const auto size = tex->size();
		if (size.x < data.tile.x || size.y < data.tile.y)
			return nullptr;

		const auto count_x = std::div(size.x, data.tile.x).quot;
		const auto count_y = std::div(size.y, data.tile.y).quot;

		SpriteList::DataType list;
		for (int i_y = 0; i_y < count_y; i_y++)
			for (int i_x = 0; i_x < count_x; i_x++)
			{
				const Vector2i pos(i_x * data.tile.x, i_y * data.tile.y);
				auto spr = std::make_shared<Sprite>(
					tex, Recti(pos, data.tile));
				list.push_back(spr);
			}

		return std::make_shared<SpriteList>(list);
	}
}