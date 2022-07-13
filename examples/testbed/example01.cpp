#include "example01.hpp"
#include "unicore/Time.hpp"
#include "unicore/Transform2.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example01, "Graphics2D");

	bool Example01::load(ResourceCache& resources)
	{
		return true;
	}

	void Example01::update()
	{
		_angle += 90_deg * time.delta().total_seconds();

		auto& size = render.screen_size();

		Transform2 tr;
		tr.move = size.cast<float>() / 2.f;
		tr.angle = _angle;
		tr.scale.x = 1 + .5f * Math::sin(time.elapsed().total_seconds());

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
		_graphics.render(render);
	}
}