#include "example02.hpp"
#include "unicore/system/StringBuilder.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/platform/Platform.hpp"
#include "unicore/renderer/Texture.hpp"
#include "unicore/renderer/Surface.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example02, "Stress test");

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

	Example02::Example02(const ExampleContext& context)
		: Example(context)
	{
	}

	void Example02::load(IResourceCache& resources)
	{
#if 1
		_tex = resources.load<Texture>("zazaka.png"_path);
#else
		if (const auto surface = resources.load<DynamicSurface>(L"zazaka.png"_path))
		{
			const auto size = surface->size();
			surface->fill(ColorConst4b::Magenta, Recti(0, size.y / 2, size.x, 2));
			_tex = renderer.create_texture(*surface);
		}
#endif
	}

	void Example02::update()
	{
		// SPAWN ENTITIES ////////////////////////////////////////////////////////////
		if (input.mouse().down(MouseButton::Left) && _tex)
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
		auto& screen_size = renderer.screen_size();
		const auto delta = static_cast<float>(time.delta().total_seconds());
		for (auto& entity : _entites)
			entity.update(screen_size, delta);

		// UPDATE SPRITE BATCH /////////////////////////////////////////////////////
		auto& size = renderer.screen_size();
		_sprite_batch.clear();

		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex, entity.center, entity.angle, entity.scale, entity.color);
		_sprite_batch.draw(_tex, { static_cast<float>(size.x) - 32, 32 });

		_sprite_batch.flush();
	}

	void Example02::draw() const
	{
		_sprite_batch.render(renderer);
	}

	void Example02::get_text(List<String32>& lines)
	{
		lines.push_back(StringBuilder::format(U"Count: {}", _entites.size()));
	}

	void Example02::on_drop_file(const Path& path)
	{
		if (const auto tex = platform.resources.load<Texture>(path))
			_tex = tex;
	}

	void Example02::spawn_entity(const Vector2f& position, const Vector2i& size)
	{
		Entity entity;
		entity.center = position;
		entity.velocity = Vector2f(
			random.range(100.f, 500.f) * random.sign<float>(),
			random.range(100.f, 500.f) * random.sign<float>()
		);
		entity.color = Color4b::create_random(random);

		entity.radius = random.range(25.f, 50.f);
		entity.scale = Vector2f(2.f * entity.radius / static_cast<float>(size.x));

		entity.angle = Degrees(random.range(359.f, 359.f));
		entity.angle_speed = Degrees(random.range(45.f, 300.f) * random.sign<float>());

		_entites.push_back(entity);
	}

	void Example02::spawn_entities(unsigned count)
	{
		auto& screen_size = renderer.screen_size();
		auto& size = _tex->size();

		for (unsigned i = 0; i < count; i++)
		{
			const Vector2f pos(
				random.range(0.f, static_cast<float>(screen_size.x)),
				random.range(0.f, static_cast<float>(screen_size.y))
			);
			spawn_entity(pos, size);
		}
	}
}