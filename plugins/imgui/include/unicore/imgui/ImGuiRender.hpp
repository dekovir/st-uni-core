#pragma once
#include "unicore/imgui/ImGuiDefs.hpp"
#include "unicore/Render2D.hpp"

namespace unicore
{
	class ImGuiRender : public Object
	{
	public:
		explicit ImGuiRender(Logger& logger);

		void init(Render& render);

		virtual void begin_frame(ImGui::IO& io) = 0;
		virtual void render(const ImDrawData* data) = 0;

	protected:
		Logger& _logger;
		Shared<Texture> _font_texture;
	};

	class ImGuiRender2D : public ImGuiRender
	{
	public:
		ImGuiRender2D(Render2D& render, Logger& logger);

		void begin_frame(ImGui::IO& io) override;

		void render(const ImDrawData* data) override;

	protected:
		Render2D& _render;

		void setup_render_state();
	};
}