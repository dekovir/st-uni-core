#include "example03.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example03, "ImGui");

	Example03::Example03(const ExampleContext& context)
		: Example(context)
	{
	}

	void Example03::update()
	{
#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)
		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
#else
		ImGui::Text("Demo window is disabled");
#endif
	}

	void Example03::draw() const
	{
	}
}