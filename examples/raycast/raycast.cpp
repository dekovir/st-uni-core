#include "raycast.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/renderer/Canvas.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/raycast/Raycast.hpp"

namespace unicore
{
	constexpr Vector2i WindowSize = Vector2i(1024, 768);
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyCore::MyCore(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "Raycast", { false, WindowSize, WindowFlags }))
	{
		init_plugins(*this);

		constexpr int side = 20;

		constexpr Cell f = { CellType::Floor };
		constexpr Cell w = { CellType::Wall };

		_map = std::make_shared<Map>(side, side);
		Canvas canvas(*_map);
		canvas.fill(Cell{ CellType::Floor });
		canvas.draw_rect({ 0, 0, side, side }, w);
		canvas.draw_line_h({ 1, 5 }, 5, w);
		canvas.draw_line_h({ 7, 5 }, 5, w);
		canvas.draw_line_h({ 13, 5 }, 5, w);

		canvas.draw_rect({ 2, 7, 5, 5 }, w);
		canvas.draw_rect({ 13, 7, 5, 5 }, w);
		canvas.draw_line_h({ 2, 9 }, 16, f);

		canvas.draw_rect({ 2, 7+6, 5, 5 }, w);
		canvas.draw_rect({ 13, 7+6, 5, 5 }, w);
		canvas.draw_line_h({ 2, 9+6 }, 16, f);

		_player = std::make_shared<Player>();
		_player->position = { 3.5f, 3.5f };
		//_player->angle = 45_deg;
	}

	void MyCore::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";

		_font = resources.create<Font>(EmptyResourceOptions{});
	}

	void MyCore::on_update()
	{
		const auto screen_size = renderer.screen_size();

#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.looper.quit();

		if (
			input.keyboard().down_changed(KeyCode::Enter) &&
			input.keyboard().mods(KeyModCombine::Alt))
		{
			auto mode = display.get_mode();
			mode.fullscreen = !mode.fullscreen;
			mode.window_size = WindowSize;
			mode.window_flags = WindowFlags;
			display.set_mode(mode);
		}
#endif

		if (_player)
		{
			constexpr float forward_speed = +2;
			constexpr float backward_speed = -1;
			constexpr auto turn_speed = 180_deg;

			const auto delta = static_cast<float>(time.delta().total_seconds());

			if (input.keyboard().down(KeyCode::W) || input.keyboard().down(KeyCode::ArrowUp))
				_player->move_forward(forward_speed * delta);

			if (input.keyboard().down(KeyCode::S) || input.keyboard().down(KeyCode::ArrowDown))
				_player->move_forward(backward_speed * delta);

			if (input.keyboard().down(KeyCode::A) || input.keyboard().down(KeyCode::ArrowLeft))
				_player->angle -= turn_speed * delta;

			if (input.keyboard().down(KeyCode::D) || input.keyboard().down(KeyCode::ArrowRight))
				_player->angle += turn_speed * delta;
		}

		_graphics.clear();
		_sprite_batch.clear();

		if (_map)
		{
			constexpr float scale = 16;

			_graphics.move({ 0, 30 });
			const auto size = _map->size();
			for (int y = 0; y < size.y; y++)
				for (int x = 0; x < size.x; x++)
				{
					Cell cell;
					if (!_map->get(x, y, cell))
						continue;

					switch (cell.type)
					{
					case CellType::Floor:
						_graphics.set_color(ColorConst4b::Gray);
						break;

					case CellType::Wall:
						_graphics.set_color(ColorConst4b::White);
						break;
					}

					const Rectf rect(scale * x, scale * y, scale, scale);
					_graphics.draw_rect(rect, true);
				}

			if (_player)
			{
				const auto pos = _player->position * scale;

				_graphics.set_color(ColorConst4b::Green);
				_graphics.draw_circle(pos, scale / 4, false, 16);
				_graphics.draw_line(pos, pos + _player->forward() * (scale / 2));

#if false
				_graphics.set_color(ColorConst4b::Red);
				const Ray2 ray{ _player->position, _player->forward() };
				Raycast::cast_ray(ray, [&](const Vector2i& index, float distance)
				{
					Cell cell;
					if (distance < 100 && _map->get(index.x, index.y, cell) && cell.type != CellType::Wall)
						return true;

					const auto point = ray.get_point(distance);
					_graphics.draw_line(ray.origin * scale, point * scale);
					return false;
			});
#else
				constexpr int rays_count = 320;
				constexpr Radians fov = 70_deg;
				constexpr Radians step = fov / rays_count;

				_graphics.set_color(ColorConst4b::Red);
				for (int i = 0; i < rays_count; i++)
				{
					const Vector2f dir = VectorConst2f::AxisX.rotate(_player->angle - fov / 2 + step * i);
					const Ray2 ray{ _player->position, dir };

					Raycast::cast_ray(ray, [&](const Vector2i& index, float distance)
					{
						Cell cell;
						if (distance < 100 && _map->get(index.x, index.y, cell) && cell.type != CellType::Wall)
							return true;

						const auto point = ray.get_point(distance);
						_graphics.draw_line(ray.origin * scale, point * scale);
						return false;
					});
				}
#endif
		}
	}

		if (_font)
			_sprite_batch.print(_font, { 0, 0 }, U"Raycast example");

		_graphics.flush();
		_sprite_batch.flush();
}

	void MyCore::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_graphics.render(renderer);

		_sprite_batch.render(renderer);
	}

	UNICORE_MAIN_CORE(MyCore);
}