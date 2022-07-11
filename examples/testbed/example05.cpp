#include "example05.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Render2D.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example05, "GridMap");

	Example05::Example05(const ExampleContext& context)
		: Example(context)
		, _topology(Vector2f(32), Vector2f(25))
		, _map(_topology, 10, 10)
	{
	}

	bool Example05::load(ResourceCache& resources)
	{
		_map.set(10, 10, CellType::Solid);

		Surface solid_sur(8, 8);
		solid_sur.fill(ColorConst4b::White);
		_solid = render.create_texture(solid_sur);

		return true;
	}

	void Example05::update()
	{
		auto& size = _map.size();

		// DRAW CELLS //////////////////////////////////////////////////////////////
		_graphics.begin();
		_graphics.offset = { 100, 100 };

		for (auto y = 0; y < size.y; y++)
			for (auto x = 0; x < size.x; x++)
			{
				CellIndex index(x, y);
				auto center = _map.topology.cell_to_pos(index);

				static List<Vector2f> points;

				points.clear();
				_map.topology.get_cell_points(index, points);
				_graphics.draw_poly_line(points, false);
			}
		_graphics.end();

		// DRAW TILES //////////////////////////////////////////////////////////////
		_sprite_batch.begin();
		for (auto y = 0; y < size.y; y++)
			for (auto x = 0; x < size.x; x++)
			{
				CellType type;
				if (!_map.get(x, y, type))
					continue;

				if (type == CellType::Empty)
					continue;

				CellIndex index(x, y);
				auto center = _map.topology.cell_to_pos(index);
				_sprite_batch.draw(_solid, center, 0_rad, VectorConst2f::One);
			}
		_sprite_batch.end();
	}

	void Example05::draw() const
	{
		_sprite_batch.render(render);
		_graphics.render(render);
	}
}