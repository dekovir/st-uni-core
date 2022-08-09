#include "example01.hpp"
#include "unicore/Time.hpp"
#include "unicore/Color3.hpp"
#include "unicore/Shapes.hpp"
#include "unicore/Transform2.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example01, "Graphics2D");

	template<typename ColorType>
	struct ColorChannel
	{
		uint8_t bits;
		uint8_t offset;

		UC_NODISCARD constexpr bool empty() const { return bits == 0; }

		UC_NODISCARD constexpr ColorType max_value() const
		{
			return static_cast<ColorType>(1 << bits) - 1;
		}

		UC_NODISCARD constexpr ColorType mask() const
		{
			return max_value() << offset;
		}

		template<typename T>
		UC_NODISCARD ColorType from_value(T value) const
		{
			if (!empty())
			{
				const auto normalized = static_cast<float>(value) / color_limits<T>::max();
				return (static_cast<ColorType>(normalized * max_value()) << offset) & mask();
			}
			return 0;
		}

		template<typename T>
		UC_NODISCARD T to_value(ColorType value) const
		{
			if (!empty())
			{
				const auto raw = (value & mask()) >> offset;
				const auto normalized = static_cast<float>(raw) / max_value();
				return static_cast<T>(normalized * color_limits<T>::max());
			}

			return 0;
		}
	};

	template<typename ColorType>
	struct ColorFormat
	{
		ColorChannel<ColorType> r, g, b, a;
	};

	// 8 bit formats
	static constexpr ColorFormat<uint8_t> PIXELFORMAT_R3G3B2 = { {3, 0}, {3, 3}, {2, 6}, {0, 0} };
	static constexpr ColorFormat<uint8_t> PIXELFORMAT_A8 = { {0, 0}, {0, 0}, {0, 0}, {8, 0} };

	// 16 bit formats
	static constexpr ColorFormat<uint16_t> PIXELFORMAT_R3G3B2A8 = { {3, 0}, {3, 3}, {2, 6}, {8, 8} };
	static constexpr ColorFormat<uint16_t> PIXELFORMAT_R5G6B5 = { {5, 0}, {6, 5}, {5, 11}, {0, 0} };
	static constexpr ColorFormat<uint16_t> PIXELFORMAT_R5G5B5A1 = { {5, 0}, {5, 5}, {5, 10}, {1, 15} };
	static constexpr ColorFormat<uint16_t> PIXELFORMAT_R4G4B4A4 = { {4, 0}, {4, 4}, {4, 8}, {4, 12} };

	// 32 bit formats
	static constexpr ColorFormat<uint32_t> PIXELFORMAT_R8G8B8A8 = { {8, 0}, {8, 8}, {8, 16}, {8, 24} };
	static constexpr ColorFormat<uint32_t> PIXELFORMAT_R10G10B10A2 = { {10, 0}, {10, 10}, {10, 20}, {2, 30} };

	// 64 bit formats
	static constexpr ColorFormat<uint64_t> PIXELFORMAT_R16G16B16A16 = { {16, 0}, {16, 16}, {16, 32}, {8, 48} };

	template<typename T, typename ColorType>
	static Color3<T> from_format3(const ColorFormat<ColorType>& format, ColorType value)
	{
		const auto r = format.r.template to_value<T>(value);
		const auto g = format.g.template to_value<T>(value);
		const auto b = format.b.template to_value<T>(value);
		return Color3<T>(r, g, b);
	}

	template<typename T, typename ColorType>
	static Color4<T> from_format4(const ColorFormat<ColorType>& format, ColorType value)
	{
		const auto r = format.r.template to_value<T>(value);
		const auto g = format.g.template to_value<T>(value);
		const auto b = format.b.template to_value<T>(value);
		const auto a = format.a.template to_value<T>(value);
		return Color4<T>(r, g, b, a);
	}

	template<typename T, typename ColorType>
	static ColorType to_format3(const ColorFormat<ColorType>& format, const Color3<T>& color)
	{
		const auto r = format.r.template from_value<T>(color.r);
		const auto g = format.g.template from_value<T>(color.g);
		const auto b = format.b.template from_value<T>(color.b);
		const auto a = format.a.template from_value<T>(Color3<T>::MaxValue);
		return r | g | b | a;
	}

	template<typename T, typename ColorType>
	static ColorType to_format4(const ColorFormat<ColorType>& format, const Color4<T>& color)
	{
		const auto r = format.r.template from_value<T>(color.r);
		const auto g = format.g.template from_value<T>(color.g);
		const auto b = format.b.template from_value<T>(color.b);
		const auto a = format.a.template from_value<T>(color.a);
		return r | g | b | a;
	}

	bool Example01::load(ResourceCache& resources)
	{
		const auto formated = to_format4(PIXELFORMAT_R8G8B8A8, ColorConst4f::Yellow);
		auto color = from_format3<float>(PIXELFORMAT_R8G8B8A8, (uint32_t)0x00FF00FF);
		return true;
	}

	void Example01::update()
	{
		_angle += 90_deg * static_cast<float>(time.delta().total_seconds());

		auto& size = renderer.screen_size();

		Transform2 tr;
		tr.move = size.cast<float>() / 2.f;
		tr.angle = _angle;
		tr.scale.x = 1 + .5f * Math::sin(static_cast<float>(time.elapsed().total_seconds()));

		static List<Vector2f> points;

		points.clear();
		Shapes::gen_ellipse(points, { 900, 900 }, { 80, 50 });

		_graphics
			.clear()
			// screen bounds
			.set_color(ColorConst4b::Magenta)
			.draw_line(VectorConst2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			// other
			.set_color(ColorConst4b::Cyan)
			.draw_rect(Recti{ 600, 100, 200, 100 }, true)
			.draw_grid({ 10, 10 }, { 10, 10 },
				[](Graphics2D& graphics, const Vector2f& center) { graphics.draw_point(center); }, { 200, 200 })
			// shapes
			.set_color(ColorConst4b::Yellow)
			.draw_triangle({ 100, 100 }, { 200, 100 }, { 100, 200 })
			.draw_quad({ 400, 400 }, { 500,500 }, { 400, 600 }, { 300, 500 })
			.draw_star({ 500, 200 }, 6, 50)
			.draw_star({ 900, 200 }, 5, 50, true)
			.draw_ellipse({ 900, 300 }, { 100, 50 })
			// rect
			.set_color(Color4b::from_rgb(0xC34288))
			.draw_rect(Rectf{ 100, 800, 100, 100 }, true)
			.set_color(Color4b::from_rgb(0xFFBD01))
			.draw_path({ {100, 800}, {200, 800}, {200, 900}, {100, 900} }, { 10 }, true)
			// path
			.set_color(ColorConst4b::Blue)
			.draw_path({ {400, 800}, {500, 850}, {600, 800}, {700, 850}, {800, 800} }, { 20 })
			// ellipse
			.set_color(Color4b::from_rgb(0xAA4F08))
			.draw_convex_poly(points)
			.set_color(Color4b::from_rgb(0xFFFFFF))
			.draw_path(points, { 2 }, true)
			// transformed
			.set_color(ColorConst4b::Green)
			.set_transform(tr)
			.draw_line(
				VectorConst2f::AxisX * -150.f,
				VectorConst2f::AxisX * +150.f)
			.draw_rect(Recti(-50, -50, 100, 100))
			.move(VectorConst2f::AxisX * -150.f)
			.draw_circle(VectorConst2f::Zero, 20, true)
			.move(VectorConst2f::AxisY * 90.f)
			.draw_circle(VectorConst2f::Zero, 20)
			.flush()
			;
	}

	void Example01::draw() const
	{
		_graphics.render(renderer);
	}
}