#include "example02.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/Texture.hpp"
#include "unicore/Surface.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Metadata.hpp"
#include "unicore/Containers.hpp"

namespace unicore
{
	/*template<> struct Metadata::Type<Path> {
		static constexpr auto name = make_const_string("Path");
		static constexpr auto members = std::make_tuple(
			Metadata::CommentAttribute("test"),
			Metadata::PropertyInfo<Path, bool>("absolute"),
			Metadata::PropertyInfo<Path, bool>("empty"),
			Metadata::PropertyInfo<Path, size_t>("hash"),
			Metadata::AttributeUsage(AttributeTargets::Class)
		);
		static constexpr auto members_count = std::tuple_size_v<decltype(members)>;
	};*/

	UNICORE_METADATA_REGISTER(Path,
		Metadata::CommentAttribute("test"),
		Metadata::PropertyInfo<Path, bool>("absolute"),
		Metadata::PropertyInfo<Path, bool>("empty"),
		Metadata::PropertyInfo<Path, size_t>("hash"),
		Metadata::AttributeUsage(AttributeTargets::Class)
	);

	template<typename ... T>
	static auto extract(const std::tuple<T...>& members,
		List<Metadata::Attribute>& attributes)
	{
		constexpr auto N = std::tuple_size_v<decltype(members)>;
		using Ids = std::make_index_sequence<N>;
	}

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
		using Type = Metadata::Type<Path>;

		constexpr auto r1 = TupleHelper::sub<2>(Type::members);
		static_assert(std::get<0>(r1).text == "test");

		using I1 = std::index_sequence<5, 1, 4>;
		using I2 = std::make_index_sequence<5>;

		constexpr auto t2 = ArrayHelper::make(5, 1, 10) + Array<int, 0>() + ArrayHelper::make(3, 4);
		static_assert(t2[4] == 4);

		//constexpr auto t3 = tuple_indices(Type::members);
		//static_assert(t3[0] == 0);

		//constexpr auto t4 = tuple_extract(Type::members, t3);

		static_assert(Type::name == "Path");
		static_assert(Type::members_count == 5);
		//constexpr auto t = std::get<0>(Type::attr).text;
		//static_assert(t == "test");
	}

	bool Example02::load(ResourceCache& resources)
	{
		_font = resources.load<Font>(L"font_004.fnt"_path);
#if 1
		_tex = resources.load<Texture>(L"zazaka.png"_path);
#else
		if (const auto surface = resources.load<DynamicSurface>(L"zazaka.png"_path))
		{
			const auto size = surface->size();
			surface->fill(ColorConst4b::Magenta, Recti(0, size.y / 2, size.x, 2));
			_tex = renderer.create_texture(*surface);
		}
#endif

		return _tex != nullptr;
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