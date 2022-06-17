#include "main.h"
#include "UnicoreMain.h"
#include "unicore/Graphics.hpp"
#include "unicore/LogHelper.hpp"
#include "unicore/Input.hpp"

namespace unicore
{
	MyCore::MyCore(Platform& _platform)
		: Core(_platform)
	{
		UC_LOG_INFO(logger) << "Info";

		if (const auto stream = file_system.open_read(L"assets/zazaka.bmp"_path))
			_tex = render.load_texture(stream);
	}

	void MyCore::on_update()
	{
		_angle += RadiansConst::Pi * static_cast<float>(time.delta().total_seconds());

		auto& size = render.screen_size();
		Graphics graphics(_queue, GraphicsFlag::CallBeginEnd);
		graphics
			.draw_tri(
				Vertex::from_pos({ 100, 100 }, ColorConst4b::Yellow),
				Vertex::from_pos({ 200, 100 }, ColorConst4b::Cyan),
				Vertex::from_pos({ 100, 200 }, ColorConst4b::Magenta))
			.set_color(ColorConst4b::Red)
			.draw_line(Vector2i::Zero, size)
			.draw_line(Vector2i(0, size.y), Vector2i(size.x, 0))
			.set_color(ColorConst4b::White)
			.draw_sprite_at(_tex, input.mouse_position().cast<float>(), _angle, { 2, 2 })
			.set_color(0x20FF00FF_rgba)
			.draw_point(Vector2f(400, 400))
			;
	}

	void MyCore::on_draw()
	{
		render.clear(ColorConst4b::Black);
		_queue.draw(render);
	}

	UNICORE_MAIN_CORE(MyCore);
}