#include "main.h"
#include "UnicoreMain.h"
#include "unicore/Graphics.hpp"

namespace unicore
{
	MyCore::MyCore(Platform& platform)
		: Core(platform)
	{
		logger.info("Start");
	}

	void MyCore::on_update()
	{
		auto& size = render.screen_size();
		Graphics graphics(_queue, GraphicsFlag::CallBeginEnd);
		graphics
			.draw_tri(
				{ Vector2f(100, 100), Colors::Yellow },
				{ Vector2f(200, 100), Colors::Cyan },
				{ Vector2f(100, 200), Colors::Magenta })
			.set_color(Colors::Red)
			.draw_line(Vector2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			;
	}

	void MyCore::on_draw()
	{
		auto& size = render.screen_size();

		render.clear(Colors::Black);
		_queue.draw(render);
	}

	UNICORE_MAIN_CORE(MyCore);
}