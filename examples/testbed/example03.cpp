#include "example03.hpp"
#include "unicore/resource/ResourceCache.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example03, "ImGui");

	Example03::Example03(const ExampleContext& context) : Example(context)
		, _imgui_logger("[IMGUI] ", logger)
		, _imgui_render(renderer, _imgui_logger)
		, _imgui(_imgui_render, time, input, _imgui_logger)
	{
		_imgui_render.init(renderer);
	}

	void Example03::update()
	{
		_imgui.frame_begin();

		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		_imgui.frame_end();
	}

	void Example03::draw() const
	{
		_imgui.render();
	}
}