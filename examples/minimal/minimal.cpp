#include "minimal.hpp"
#include "UnicoreMain.h"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	constexpr Vector2i WindowSize = Vector2i(800, 600);
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	void Entity::update(const Vector2i& size, float delta)
	{
		center += velocity * delta;

		if (
			(velocity.x < 0 && center.x < radius) ||
			(velocity.x > 0 && center.x > size.x - radius))
		{
			velocity.x *= -1;
		}
		if (
			(velocity.y < 0 && center.y < radius) ||
			(velocity.y > 0 && center.y > size.y - radius))
		{
			velocity.y *= -1;
		}

		angle += angle_speed * delta;
	}

	MyCore::MyCore(const CoreSettings& settings)
		: SDLCore(create_settings(settings, "Minimal", { false, WindowSize, WindowFlags }))
	{
	}

	void MyCore::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";

		constexpr auto side = 64;
		constexpr auto radius_outer = side / 2;
		constexpr auto radius_inner = radius_outer / 6;
		constexpr Vector2i center(radius_outer, radius_outer);

		DynamicSurface circle(side, side);
		Canvas canvas(circle);

#if 1
		canvas.fill([radius_outer, radius_inner, &center](Canvas<Color4b>& canvas, const Vector2i& pos) -> bool
			{
				const float distance = center.distance(pos);
				if (distance < radius_inner || distance > radius_outer)
					canvas.draw_point(pos, ColorConst4b::Clear);
				else
				{
					const float t = 1 - Math::sin(Math::Pi * (distance - radius_inner) / (radius_outer - radius_inner));
					const auto color = Color4b::lerp(ColorConst4b::White, ColorConst4b::Clear, t);
					canvas.draw_point(pos, color);
				}

				return true;
			});

		canvas.fill_rect(Recti(0, radius_outer - 3, side, 6), ColorConst4b::Clear);
		canvas.fill_rect(Recti(radius_outer - 3, 0, 6, side), ColorConst4b::Clear);
#else
		canvas.fill(ColorConst4b::Clear);
		for (unsigned i = 0; i < 360; i += 15)
		{
			const auto pos = Vector2i(0, radius_outer).rotate(Degrees(static_cast<float>(i)));
			canvas.draw_line(center, center + pos, ColorConst4b::White);
		}

		constexpr auto step = radius_outer / 8;

		for (unsigned i = step; i <= radius_outer; i += step)
			canvas.draw_circle(center, i - 1, ColorConst4b::White);
#endif

		_tex = renderer.create_texture(circle);
	}

	void MyCore::on_update()
	{
		const auto screen_size = renderer.screen_size();

#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();

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

		_add_active = input.mouse().down(MouseButton::Left);

		// SPAWN ENTITIES ////////////////////////////////////////////////////////////
		if (_add_active && _tex)
		{
			_add_time += time.delta();
			constexpr auto time_period = TimeSpan::from_microseconds(1000);
			while (_add_time >= time_period)
			{
				spawn_entity(input.mouse().position().cast<float>(), _tex->size());
				_add_time -= time_period;
			}
		}
		else _add_time = TimeSpanConst::Zero;

		// UPDATE ENTITIES ///////////////////////////////////////////////////////////
		if (!input.keyboard().down(KeyCode::Space))
		{
			const auto delta = static_cast<float>(time.delta().total_seconds());
			for (auto& entity : _entites)
				entity.update(screen_size, delta);
		}

		// GRAPHICS ////////////////////////////////////////////////////////////////
		_graphics
			.clear()
			.set_color(ColorConst4b::Magenta)
			.draw_line(VectorConst2i::Zero, screen_size)
			.draw_line(Vector2i(0, screen_size.y), Vector2i(screen_size.x, 0))
			.flush()
			;

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		_sprite_batch.clear();

		_sprite_batch.draw(_tex, (screen_size / 2).cast<float>());
		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex, entity.center, entity.angle, entity.scale, entity.color);

		_sprite_batch.flush();
	}

	void MyCore::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_graphics.render(renderer);

		_sprite_batch.render(renderer);
	}

	void MyCore::spawn_entity(const Vector2f& position, const Vector2i& size)
	{
		Entity entity;
		entity.center = position;
		entity.velocity = Vector2f(
			_random.range(100.f, 500.f) * _random.sign<float>(),
			_random.range(100.f, 500.f) * _random.sign<float>()
		);
		entity.color = Color4b::create_random(_random);

		entity.radius = _random.range(25.f, 50.f);
		entity.scale = Vector2f(2.f * entity.radius / static_cast<float>(size.x));

		entity.angle = Degrees(_random.range(359.f, 359.f));
		entity.angle_speed = Degrees(_random.range(45.f, 300.f) * _random.sign<float>());

		_entites.push_back(entity);
	}

	void MyCore::spawn_entities(unsigned count)
	{
		if (!_tex) return;

		auto& screen_size = renderer.screen_size();
		auto& size = _tex->size();

		for (unsigned i = 0; i < count; i++)
		{
			const Vector2f pos(
				_random.range(0.f, static_cast<float>(screen_size.x)),
				_random.range(0.f, static_cast<float>(screen_size.y))
			);
			spawn_entity(pos, size);
		}
	}

	UNICORE_MAIN_CORE(MyCore);
}