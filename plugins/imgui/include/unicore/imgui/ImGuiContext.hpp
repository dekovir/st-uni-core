#pragma once
#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/platform/Input.hpp"

namespace unicore
{
	class ImGuiContext
	{
	public:
		ImGuiContext(ImGuiRender& render, Time& time, Input& input, Logger& logger);
		~ImGuiContext();

		void frame_begin();
		void frame_end();

		void render() const;

		UC_NODISCARD bool is_mouse_over() const;

	protected:
		ImGui::Context* _handle;
		ImGuiRender& _render;
		Time& _time;
		Input& _input;
		Logger& _logger;
	};
}