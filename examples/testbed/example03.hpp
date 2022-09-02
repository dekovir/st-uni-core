#pragma once
#include "example.hpp"
#include "unicore/Logger.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	class Example03 : public Example
	{
	public:
		explicit Example03(const ExampleContext& context);

		void update() override;
		void draw() const override;

	protected:
		ProxyLogger _imgui_logger;
		ImGuiRender2D _imgui_render;
		ImGuiContext _imgui;
	};
}