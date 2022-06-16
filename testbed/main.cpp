#include "main.h"
#include "UnicoreMain.h"
#include "unicore/Graphics.hpp"

namespace unicore
{
	MyCore::MyCore(Platform& platform)
		: Core(platform)
	{
		logger.info("Start");

		if (const auto stream = file_system.open_read(Path(L"assets/zazaka.bmp")))
			_tex = render.load_texture(stream);
	}

	void MyCore::on_update()
	{
		auto& size = render.screen_size();
		Graphics graphics(_queue, GraphicsFlag::CallBeginEnd);
		graphics
			.draw_tri(
				Vertex::from_pos(Vector2f(100, 100), Colors4b::Yellow),
				Vertex::from_pos(Vector2f(200, 100), Colors4b::Cyan),
				Vertex::from_pos(Vector2f(100, 200), Colors4b::Magenta))
			.set_color(Colors4b::Red)
			.draw_line(Vector2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			.set_color(Colors4b::White)
			.draw_sprite(_tex, Vector2f(400, 400))
			;
	}

	void MyCore::on_draw()
	{
		auto& size = render.screen_size();

		render.clear(Colors4b::Black);
		_queue.draw(render);
	}

	UNICORE_MAIN_CORE(MyCore);
}