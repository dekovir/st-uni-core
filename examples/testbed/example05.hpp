#pragma once
#include "example.hpp"
#include "unicore/Transform2.hpp"
#include "unicore/Sprite.hpp"
#include "unicore/SpriteBatch.hpp"
#include "unicore/Graphics2D.hpp"
#include "unicore/GridMap.hpp"
#include "unicore/Sprite.hpp"

namespace unicore
{
	enum class CellType : uint16_t
	{
		Empty = 0,
		Grass = 1,
		Solid = 7,
	};

	class Example05 : public Example
	{
	public:
		explicit Example05(const ExampleContext& context);

		void load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		Shared<SpriteList> _tiles;
		Shared<Font> _font;

		RectangleTopology _topology;
		GridMap<CellType> _map;
		Transform2 _tr;

		Graphics2D _graphics;
		SpriteBatch _sprite_batch;
	};
}