#include "example02.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Reflection.hpp"
#include "unicore/Containers.hpp"

namespace unicore
{
	template<typename ... Ts, typename Tuple>
	constexpr auto tuple_extract(Tuple tuple)
	{
		auto get_element = [](auto el) {
			if constexpr ((std::is_base_of_v<decltype(el), Ts> || ...)) {
				return std::make_tuple(std::move(el));
			}
			else {
				return std::make_tuple();
			}
		};
		return std::apply([&](auto ... args)
			{
				return std::tuple_cat(get_element(std::move(args)) ...);
			}
			, std::move(tuple)
				);
	}



	template<> struct Reflection::Type<Path> {
		static constexpr auto name = make_const_string("Path");
		static constexpr auto members = std::make_tuple(
			Reflection::CommentAttribute("test"),
			Reflection::PropertyInfo<Path, bool>("absolute"),
			Reflection::PropertyInfo<Path, bool>("empty"),
			Reflection::PropertyInfo<Path, size_t>("hash"),
			Reflection::AttributeUsage(AttributeTargets::Class)
		);
		static constexpr auto members_count = std::tuple_size_v<decltype(members)>;
		static constexpr auto attr = tuple_extract<Attribute>(members);
		static constexpr auto attr_count = std::tuple_size_v<decltype(attr)>;
	};

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
		using Type = Reflection::Type<Path>;

		constexpr auto r1 = TupleHelper::sub<2>(Type::members);
		static_assert(std::get<0>(r1).text == "test");

		using I1 = std::index_sequence<5, 1, 4>;

		constexpr auto t2 = ArrayHelper::make(5, 1, 10) + Array<int, 0>() + ArrayHelper::make(3, 4);
		//constexpr auto t3 = std::get<std::get<0>(t2)>(Type::members);
		static_assert(t2[4] == 4);

		static_assert(Type::name == "Path");
		static_assert(Type::members_count == 5);
		//constexpr auto t = std::get<0>(Type::attr).text;
		//static_assert(t == "test");
	}

	bool Example02::load(ResourceCache& resources)
	{
		_font = resources.load<Font>(L"font_004.fnt"_path);
		_tex = resources.load<Texture>(L"zazaka.png"_path);

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
		auto& screen_size = renderer.screen_size();
		const auto delta = static_cast<float>(time.delta().total_seconds());
		for (auto& entity : _entites)
			entity.update(screen_size, delta);

		// UPDATE SPRITE BATCH /////////////////////////////////////////////////////
		const auto count_str = StringBuilder::format("Count: {}", _entites.size());

		auto& size = renderer.screen_size();
		_sprite_batch.clear();

		for (const auto& entity : _entites)
			_sprite_batch.draw(_tex, entity.center, entity.angle, entity.scale, entity.color);
		_sprite_batch.draw(_tex, { static_cast<float>(size.x) - 32, 32 });

		_sprite_batch.print(_font, { 200, 20 }, count_str);

		_sprite_batch.flush();
	}

	void Example02::draw() const
	{
		_sprite_batch.render(renderer);
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