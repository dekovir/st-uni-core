#include "example06.hpp"
#include "unicore/Input.hpp"
#include "unicore/Font.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/stb/StbTTFont.hpp"

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
		//_font = resources.create<Font>(42);
		//_font = resources.load<Font>(L"font_004.fnt"_path);
		//_font = resources.load<Font>(L"ubuntu.regular.ttf"_path);
		_font = resources.create<Font>(
			TTFOptions{ L"ubuntu.regular.ttf"_path, 24,
				(CharTable::Ascii + CharTable::Russian).view() });
	}

	void Example06::update()
	{
		constexpr WStringView text = L"Hello world!\nSecond line\nThird line\n\nLast one";

		const auto screen_size = renderer.screen_size().cast<Float>();
		const auto center_screen = screen_size / 2.f;

		_graphics.clear();
		_graphics.set_color(ColorConst4b::Magenta);
		_graphics.draw_line(VectorConst2f::Zero, screen_size);
		_graphics.draw_line(Vector2f(screen_size.x, 0), Vector2f(0, screen_size.y));
		_graphics.flush();

		_sprite_batch.clear();

		if (const auto fnt = std::dynamic_pointer_cast<StbTTFont>(_font))
		{
			const auto size = fnt->texture()->size().cast<Float>();
			const auto center = Vector2f(screen_size.x - size.x / 2, size.y / 2);
			_sprite_batch.draw(fnt->texture(), center);
		}

		_sprite_batch.print({ _font, text, TextAlign::Center }, center_screen);
		_sprite_batch.flush();
	}

	void Example06::draw() const
	{
		_graphics.render(renderer);
		_sprite_batch.render(renderer);
	}
}