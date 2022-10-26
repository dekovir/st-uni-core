#include "example03.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example03, "ImGui");

	Example03::Example03(const ExampleContext& context)
		: Example(context)
	{
	}

	void Example03::update()
	{
		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	void Example03::draw() const
	{
	}
}