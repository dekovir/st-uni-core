#include "example05.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/Surface.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Texture.hpp"
#include "unicore/experimental/ResourcePath.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example05, "GridMap");

	namespace foobar
	{
		enum class ValueType { Int, Float };

		struct Value
		{
			ValueType type;
		};

		struct IntValue : Value
		{
			Int value;
		};

		struct FloatValue : Value
		{
			Float value;
		};

		namespace internal
		{
			template<typename T>
			struct convert {};

			template<>
			struct convert<Int>
			{
				constexpr IntValue operator()(Int value) const noexcept
				{
					return { {ValueType::Int}, value };
				}
			};

			template<>
			struct convert<float>
			{
				constexpr FloatValue operator()(Float value) const noexcept
				{
					return { {ValueType::Float}, value };
				}
			};
		}

		template<typename T>
		static constexpr auto to_value(T value) noexcept
		{
			internal::convert<T> convert;
			return convert(value);
		}

		//template<typename ... Args>
		//static constexpr auto make(Args... args) noexcept
		//{
		//	return std::make_pair(to_value<Args>(args)...);
		//}
	}

	namespace parser
	{
		struct Result
		{
			StringView name;
			StringView value;
		};

		template<size_t N>
		struct Table
		{
			Result value[N];

			constexpr Result operator[](int index) const { return value[index]; }
		};

		static constexpr Table<2> operator+(const Result& a, const Result& b)
		{
			return { a, b };
		}

		template<size_t N1, size_t N2>
		static constexpr Table<N1 + N2> operator+(const Table<N1>& a, const Table<N2>& b)
		{
			Table<N1 + N2> data{};
			for (size_t i = 0; i < N1; i++)
				data.value[i] = a.value[i];
			for (size_t i = 0; i < N2; i++)
				data.value[N1 + i] = b.value[i];
			return data;
		}

		namespace internal
		{
			static constexpr Result parse_one(StringView text)
			{
				const auto pos = text.find_first_of('=');
				if (pos != StringView::npos)
					return { text.substr(0, pos), text.substr(pos + 1) };

				return { text, StringView() };
			}
		}
	}

	Example05::Example05(const ExampleContext& context)
		: Example(context)
		, _topology(Vector2f(32))
		, _map(30, 30, _topology)
		, _tr({ 100, 100 }, 10_deg)
	{
		constexpr auto a = parser::internal::parse_one("name=value");
		constexpr auto b = parser::internal::parse_one("height=32");
		constexpr auto table = a + b;

		static_assert(table.value[0].name == "name");
		static_assert(table.value[1].value == "32");

		constexpr auto v = foobar::to_value(12);
		//constexpr auto t = foobar::make(12);

		constexpr auto p = ResourcePath::make("create:font?height=32");
		static_assert(p.base() == "create");
		static_assert(p.data() == "font");
		static_assert(p.args() == "height=32");
	}

	void Example05::load(ResourceCache& resources)
	{
		_tiles = resources.create<SpriteList>(
			CreateResource::TileSet{ L"tiles.png"_path, Vector2i(16) });

		_map.fill(CellType::Grass);
		_map.fill(CellType::Solid, Recti(1, 1, 5, 5));
		_map.set(10, 10, CellType::Solid);

		_font = resources.load<Font>(L"ubuntu.regular.ttf"_path);
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

		_sprite_batch.print(_font, { 400, 400 }, L"Hello world!", ColorConst4b::Black);

		_sprite_batch.flush();
	}

	void Example05::draw() const
	{
		_sprite_batch.render(renderer);
		_graphics.render(renderer);
	}
}