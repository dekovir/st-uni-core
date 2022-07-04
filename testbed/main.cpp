#include "main.h"
#include "UnicoreMain.h"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Surface.hpp"

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

		_tex1 = resources.load<Texture>(L"assets/zazaka.png"_path);

		{
			constexpr auto side = 256;
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

			_tex2 = render.create_texture(circle);
		}

		_font = resources.load<BitmapFont>(L"assets/font_004.fnt"_path);

		size_t sys_mem;
		resources.calc_memory_use(&sys_mem, nullptr);
		UC_LOG_DEBUG(logger) << "Resource used system memory " << MemorySize{ sys_mem };

		resources.unload_unused();
		resources.dump_used();
	}

	void MyCore::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.key_code(KeyCode::Escape))
			platform.quit();
#endif

		_add_active = input.mouse_button(MouseButton::Left);

		// SPAWN ENTITIES ////////////////////////////////////////////////////////////
		if (_add_active && _tex1)
		{
			_add_time += time.delta();

			constexpr auto time_period = TimeSpan::from_microseconds(1000);
			while (_add_time >= time_period)
			{
				spawn_entity(input.mouse_position().cast<float>(), _tex1->size());
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
			.draw_tri(
				{ { 100, 100 }, ColorConst4b::Yellow },
				{ { 200, 100 }, ColorConst4b::Cyan },
				{ { 100, 200 }, ColorConst4b::Magenta })
			.set_color(ColorConst4b::Magenta)
			.draw_line(VectorConst2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			.set_color(ColorConst4b::Cyan)
			.draw_rect(Recti{ 600, 100, 200, 100 }, true)
			.draw_point(Vector2i{ 300, 300 })
			;

		_graphics.end();

		// SPRITE BATCH ////////////////////////////////////////////////////////////
		const String fps_str = "FPS: " + std::to_string(fps());
		const String count_str = "Count: " + std::to_string(_entites.size());
		const String draw_str = "Draw: " + std::to_string(_draw_calls);

		_sprite_batch.begin();

		_sprite_batch.draw(_tex2, { static_cast<float>(size.x) / 2, static_cast<float>(size.y) / 2 });

		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex1, entity.center, entity.angle, entity.scale, entity.color);
		_sprite_batch.draw(_tex1, { static_cast<float>(size.x) - 32, 32 });

		_sprite_batch.print(_font, { 0, 0 }, fps_str);
		_sprite_batch.print(_font, { 0, 20 }, count_str);
		_sprite_batch.print(_font, { 0, 40 }, draw_str);

		_sprite_batch.end();
	}

	void MyCore::on_draw()
	{
		render.clear(ColorConst4b::Black);

		_graphics.render(render);

		_sprite_batch.render(render);

		_draw_calls = render.draw_calls();
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
		auto& screen_size = render.screen_size();
		auto& size = _tex1->size();

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