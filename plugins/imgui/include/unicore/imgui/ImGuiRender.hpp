#pragma once
#include "unicore/imgui/ImGuiDefs.hpp"
#include "unicore/Renderer2D.hpp"

namespace unicore
{
	class ImGuiRender : public Object
	{
		UC_OBJECT(ImGuiRender, Object)
	public:
		explicit ImGuiRender(Logger& logger);

		void init(Renderer& render);

		virtual void begin_frame(ImGui::IO& io) = 0;
		virtual void render(const ImDrawData* data) = 0;

	protected:
		Logger& _logger;
		Shared<Texture> _font_texture;
	};

	class ImGuiRender2D : public ImGuiRender
	{
		UC_OBJECT(ImGuiRender2D, Object)
	public:
		ImGuiRender2D(Renderer2D& render, Logger& logger);

		void begin_frame(ImGui::IO& io) override;

		void render(const ImDrawData* data) override;

	protected:
		Renderer2D& _render;

		void setup_render_state();
	};
}