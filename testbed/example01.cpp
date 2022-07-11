#include "example01.hpp"
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
		auto& size = render.screen_size();

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
	}

	void Example01::draw() const
	{
		_graphics.render(render);
	}
}