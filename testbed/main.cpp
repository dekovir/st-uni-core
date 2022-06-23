#include "main.h"
#include "UnicoreMain.h"
#include "unicore/LogHelper.hpp"
#include "unicore/Input.hpp"
#include "unicore/Graphics.hpp"
#include "unicore/SpriteBatch.hpp"

namespace unicore
{
	MyCore::MyCore(Platform& _platform)
		: Core(_platform)
	{
		UC_LOG_INFO(logger) << "Info";

_tex = resources.load<Texture>(L"assets/zazaka.bmp"_path);
	}

	void MyCore::on_update()
	{
		_angle += 180_deg * static_cast<float>(time.delta().total_seconds());
	}

	void MyCore::on_draw()
	{
		auto& size = render.screen_size();

		render.clear(ColorConst4b::Black);
		Graphics graphics;
		graphics.begin();
		graphics
			.draw_tri(
				Vertex2::from_pos({ 100, 100 }, ColorConst4b::Yellow),
				Vertex2::from_pos({ 200, 100 }, ColorConst4b::Cyan),
				Vertex2::from_pos({ 100, 200 }, ColorConst4b::Magenta))
			.set_color(ColorConst4b::Red)
			.draw_line(Vector2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			.set_color(ColorConst4b::White)
			.set_color(0x20FF00FF_rgba)
			.draw_point(Vector2f(400, 400))
			;

		graphics.end();
		graphics.to_render(render);

		SpriteBatch batch;
		batch.begin();
		batch.draw(_tex, input.mouse_position().cast<float>(), _angle, { 2, 2 });
		batch.end();
		batch.to_render(render);
	}

	UNICORE_MAIN_CORE(MyCore);
}