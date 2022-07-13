#pragma once
#include "example.hpp"
#include "unicore/Transform2.hpp"
#include "unicore/Sprite.hpp"
#include "unicore/SpriteBatch.hpp"
#include "unicore/Graphics2D.hpp"
#include "unicore/gridmap/GridMap.hpp"

namespace unicore
{
	enum class CellType
	{
		Empty,
		Solid,
	};

	class Example05 : public Example
	{
	public:
		explicit Example05(const ExampleContext& context);

		bool load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		RectangleTopology _topology;
		Array<Shared<Sprite>, 2> _tiles;
		GridMap<CellType> _map;
		Transform2 _tr;

		Graphics2D _graphics;
		SpriteBatch _sprite_batch;
	};
}