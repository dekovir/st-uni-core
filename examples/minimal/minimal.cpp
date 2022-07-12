#include "minimal.hpp"
#include "UnicoreMain.h"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
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
		: Core(settings)
	{
		UC_LOG_INFO(logger) << "Starting";

		constexpr auto side = 64;
		constexpr auto radius_outer = side / 2;
		constexpr auto radius_inner = radius_outer / 6;
		constexpr Vector2i center(radius_outer, radius_outer);

		Surface circle(side, side);

		circle.fill([radius_outer, radius_inner, &center](int x, int y) -> Color4b
			{
				const float distance = center.distance({ x, y });
				if (distance < radius_inner || distance > radius_outer)
					return ColorConst4b::Clear;

				const float t = 1 - Math::sin(Math::PI * (distance - radius_inner) / (radius_outer - radius_inner));
				return Color4b::lerp(ColorConst4b::White, ColorConst4b::Clear, t);
			});

		_tex = render.create_texture(circle);
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.quit();
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
		auto& screen_size = render.screen_size();
		const auto delta = static_cast<float>(time.delta().total_seconds());
		for (auto& entity : _entites)
			entity.update(screen_size, delta);

		auto& size = render.screen_size();

		// GRAPHICS ////////////////////////////////////////////////////////////////
		_graphics.begin();
		_graphics
			.set_color(ColorConst4b::Magenta)
			.draw_line(VectorConst2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			.set_color(ColorConst4b::Cyan)
			.draw_rect(Recti{ 600, 100, 200, 100 }, true)
			.draw_point(Vector2i{ 300, 300 })
			;

		_graphics.end();

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		_sprite_batch.begin();

		_sprite_batch.draw(_tex, (size / 2).cast<float>());
		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex, entity.center, entity.angle, entity.scale, entity.color);

		_sprite_batch.end();
	}

	void MyCore::on_draw()
	{
		render.clear(ColorConst4b::Black);

		_graphics.render(render);

		_sprite_batch.render(render);
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

		auto& screen_size = render.screen_size();
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