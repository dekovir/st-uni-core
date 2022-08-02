#include "example01.hpp"
#include "unicore/Time.hpp"
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

	static constexpr ColorFormat<uint16_t> RGB_565 = { {5, 0}, {6, 5}, {5, 11}, {0, 0} };

	template<typename T, typename ColorType>
	static Color4<T> from_format(const ColorFormat<ColorType>& format, ColorType value)
	{
		const auto r = format.r.template to_value<T>(value);
		const auto g = format.g.template to_value<T>(value);
		const auto b = format.b.template to_value<T>(value);
		const auto a = format.a.template to_value<T>(value);
		return Color4<T>(r, g, b, a);
	}

	template<typename T, typename ColorType>
	static ColorType to_format(const ColorFormat<ColorType>& format, const Color4<T>& color)
	{
		const auto r = format.r.template from_value<T>(color.r);
		const auto g = format.g.template from_value<T>(color.g);
		const auto b = format.b.template from_value<T>(color.b);
		const auto a = format.a.template from_value<T>(color.a);
		return r | g | b | a;
	}

	bool Example01::load(ResourceCache& resources)
	{
		const auto formated = to_format(RGB_565, ColorConst4b::Yellow);
		auto color = from_format<float>(RGB_565, formated);
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

		_graphics.begin();
		_graphics
			// screen bounds
			.set_color(ColorConst4b::Magenta)
			.draw_line(VectorConst2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			// other
			.set_color(ColorConst4b::Cyan)
			.draw_rect(Recti{ 600, 100, 200, 100 }, true)
			.draw_point(Vector2i{ 300, 300 })
			// transformed
			.set_color(ColorConst4b::Green)
			.set_transform(tr)
			.draw_line(
				VectorConst2f::AxisX * -150.f,
				VectorConst2f::AxisX * +150.f)
			.draw_rect(Recti(-50, -50, 100, 100))
			.move(VectorConst2f::AxisX * -150.f)
			.draw_circle(VectorConst2f::Zero, 20)
			.move(VectorConst2f::AxisY * 90.f)
			.draw_circle(VectorConst2f::Zero, 20)
			;

		_graphics.end();
	}

	void Example01::draw() const
	{
		_graphics.render(renderer);
	}
}