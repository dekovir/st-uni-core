#include "example05.hpp"

#include "unicore/Input.hpp"
#include "unicore/Surface.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example05, "GridMap");

	Example05::Example05(const ExampleContext& context)
		: Example(context)
		, _topology(Vector2f(32))
		, _map(30, 30, _topology)
		, _tr({ 100, 100 }, 10_deg)
	{
	}

	bool Example05::load(ResourceCache& resources)
	{
		_map.fill(CellType::Solid, Recti(1, 1, 5, 5));
		_map.set(10, 10, CellType::Solid);

		Surface solid_sur(8, 8);
		solid_sur.fill(ColorConst4b::Green);

		if (auto texture = renderer.create_texture(solid_sur))
			_tiles[static_cast<int>(CellType::Solid)] = std::make_shared<Sprite>(texture);

		return true;
	}

	void Example05::update()
	{
		static List<Vector2f> points;
		auto& size = _map.size();

		// DRAW CELLS //////////////////////////////////////////////////////////////
		_graphics.begin();
		_graphics.set_transform(_tr);

		_graphics.set_color(ColorConst4b::White);
		for (auto y = 0; y < size.y; y++)
			for (auto x = 0; x < size.x; x++)
			{
				const CellIndex index(x, y);

				points.clear();
				_map.topology.get_cell_points(index, points);
				_graphics.draw_poly_line(points, true);
			}

		_graphics.set_color(ColorConst4b::Magenta);
		for (auto y = 0; y < size.y; y++)
			for (auto x = 0; x < size.x; x++)
			{
				const CellIndex index(x, y);
				const auto pos = _map.topology.cell_to_pos(index);
				_graphics.draw_point(pos);
			}
		_graphics.end();

		// DRAW TILES //////////////////////////////////////////////////////////////

		_sprite_batch.clear();
		for (auto y = 0; y < size.y; y++)
			for (auto x = 0; x < size.x; x++)
			{
				CellType type;
				if (!_map.get(x, y, type))
					continue;

				auto tile = _tiles[static_cast<int>(type)];
				if (!tile)
					continue;

				const auto scale = _map.topology.size() / tile->rect().size().cast<float>();
				const CellIndex index(x, y);
				auto center = _map.topology.cell_to_pos(index);
				_sprite_batch.draw(tile, _tr * center, _graphics.transform.angle, scale);
			}
		_sprite_batch.flush();
	}

	void Example05::draw() const
	{
		_sprite_batch.render(renderer);
		_graphics.render(renderer);
	}
}