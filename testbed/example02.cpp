#include "example02.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/Texture.hpp"
#include "unicore/Render2D.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example02, "SpriteBatch");

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

	bool Example02::load(ResourceCache& resources)
	{
		_font = resources.load<Font>(L"assets/font_004.fnt"_path);
		_tex = resources.load<Texture>(L"assets/zazaka.png"_path);

		return _tex != nullptr;
	}

	void Example02::update()
	{
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

		// UPDATE SPRITE BATCH /////////////////////////////////////////////////////
		const String count_str = "Count: " + std::to_string(_entites.size());

		auto& size = render.screen_size();
		_sprite_batch.begin();

		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex, entity.center, entity.angle, entity.scale, entity.color);
		_sprite_batch.draw(_tex, { static_cast<float>(size.x) - 32, 32 });

		_sprite_batch.print(_font, { 200, 20 }, count_str);

		_sprite_batch.end();
	}

	void Example02::draw() const
	{
		_sprite_batch.render(render);
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
		auto& screen_size = render.screen_size();
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