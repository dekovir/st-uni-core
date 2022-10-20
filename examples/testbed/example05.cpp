#include "example05.hpp"
#if defined(UNICORE_USE_GRID)
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/renderer/Canvas.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Texture.hpp"

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

	void Example05::load(IResourceCache& resources)
	{
		_tiles = resources.load<SpriteList>("tiles.png"_path,
			TileSetOptions{ Vector2i(16) });

		Canvas canvas(_map);

		canvas.fill(CellType::Grass);
		canvas.fill_rect(Recti(1, 1, 5, 5), CellType::Solid);
		canvas.draw_point({ 10, 10 }, CellType::Solid);

		_font = resources.load<Font>("ubuntu.regular.ttf"_path);
	}

	void Example05::update()
	{
		static List<Vector2f> points;
		const auto map_size = _map.size();

		// DRAW CELLS //////////////////////////////////////////////////////////////
		_graphics.clear();
		_graphics.set_transform(_tr);

		_graphics.set_color(ColorConst4b::White);
		for (auto y = 0; y < map_size.y; y++)
			for (auto x = 0; x < map_size.x; x++)
			{
				const GridIndex index(x, y);

				points.clear();
				_map.topology.get_cell_points(index, points);
				_graphics.draw_poly_line(points, true);
			}

		_graphics.set_color(ColorConst4b::Magenta);
		for (auto y = 0; y < map_size.y; y++)
			for (auto x = 0; x < map_size.x; x++)
			{
				const GridIndex index(x, y);
				const auto pos = _map.topology.cell_to_pos(index);
				_graphics.draw_point(pos);
			}
		_graphics.flush();

		// DRAW TILES //////////////////////////////////////////////////////////////
		_sprite_batch.clear();

		if (_tiles && _tiles->size() > 0)
		{
			const auto screen_size = renderer.screen_size();
			const auto tex = (*_tiles)[0]->texture();
			const auto size = tex->size();
			_sprite_batch.draw(tex, Vector2f(screen_size.x - size.x / 2, size.y / 2));
		}

		for (auto y = 0; y < map_size.y; y++)
			for (auto x = 0; x < map_size.x; x++)
			{
				CellType type;
				if (!_map.get(x, y, type) || type == CellType::Empty)
					continue;

				auto tile = (*_tiles)[static_cast<int>(type)];
				if (!tile)
					continue;

				const auto scale = _map.topology.size() / tile->rect().size().cast<float>();
				const GridIndex index(x, y);
				auto center = _map.topology.cell_to_pos(index);
				_sprite_batch.draw(tile, _tr * center, _graphics.transform.angle, scale);
			}

		_sprite_batch.print(_font, { 400, 400 }, U"Hello world!", ColorConst4b::Black);

		_sprite_batch.flush();
	}

	void Example05::draw() const
	{
		_sprite_batch.render(renderer);
		_graphics.render(renderer);
	}
}
#endif