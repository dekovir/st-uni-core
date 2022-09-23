#include "example06.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example06, "Fonts");

	Example06::Example06(const ExampleContext& context)
		: Example(context)
		, _font(context.font)
	{
	}

	void Example06::load(ResourceCache& resources)
	{
		//_font = resources.load<Font>(L"ubuntu.regular.ttf"_path);
	}

	void Example06::update()
	{
		constexpr WStringView text = L"Hello world!\nSecond line\nThird line";

		const auto screen_size = renderer.screen_size().cast<Float>();
		const auto center_screen = screen_size / 2.f;

		_graphics.clear();
		_graphics.set_color(ColorConst4b::Magenta);
		_graphics.draw_line(VectorConst2f::Zero, screen_size);
		_graphics.draw_line(Vector2f(screen_size.x, 0), Vector2f(0, screen_size.y));

		_graphics.flush();

		_sprite_batch.clear();

		const TextBlock block(_font, text);

		_sprite_batch.print(block, center_screen, TextAlign::Center);

		_sprite_batch.flush();
	}

	void Example06::draw() const
	{
		_graphics.render(renderer);
		_sprite_batch.render(renderer);
	}
}