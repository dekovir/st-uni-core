#pragma once
#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/Time.hpp"
#include "unicore/Input.hpp"

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

	protected:
		ImGui::Context* _handle;
		ImGuiRender& _render;
		Time& _time;
		Input& _input;
		Logger& _logger;
	};
}