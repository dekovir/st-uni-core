#include "example05.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Render2D.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example05, "GridMap");

	Example05::Example05(const ExampleContext& context)
		: Example(context)
		, _topology(Vector2f(32))
		, _map(_topology, 30, 30)
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
		static List<Vector2f> points;
		auto& size = _map.size();

		// DRAW CELLS //////////////////////////////////////////////////////////////
		_graphics.begin();
		_graphics.offset = { 100, 100 };

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
		if (_solid)
		{
			const auto scale = _map.topology.size().cast<float>() / _solid->size().cast<float>();

			_sprite_batch.begin();
			for (auto y = 0; y < size.y; y++)
				for (auto x = 0; x < size.x; x++)
				{
					CellType type;
					if (!_map.get(x, y, type))
						continue;

					if (type == CellType::Empty)
						continue;

					const CellIndex index(x, y);
					auto center = _map.topology.cell_to_pos(index);
					_sprite_batch.draw(_solid, center + _graphics.offset, 0_rad, scale);
				}
			_sprite_batch.end();
		}
	}

	void Example05::draw() const
	{
		_sprite_batch.render(render);
		_graphics.render(render);
	}
}