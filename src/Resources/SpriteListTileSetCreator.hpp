#pragma once
#include "unicore/ResourceCreator.hpp"
#include "unicore/Sprite.hpp"

namespace unicore
{
	class SpriteListTileSetCreator : public ResourceCreatorData<SpriteList, CreateResource::TileSet>
	{
		UC_OBJECT(SpriteListTileSetCreator, ResourceCreatorData)
	public:

	protected:
		Shared<SpriteList> create_from_data(const Options& options,
			const CreateResource::TileSet& data) override;
	};
}