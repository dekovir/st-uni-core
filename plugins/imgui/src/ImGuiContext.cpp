#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	ImGuiContext::ImGuiContext(ImGuiRender& render, Time& time, Input& input, Logger& logger)
		: _handle(ImGui::CreateContext()), _render(render), _time(time), _input(input), _logger(logger)
	{
		ImGui::SetCurrentContext(_handle);

		ImGui::StyleColorsDark();
		auto& io = ImGui::GetIO();

		// BACKEND PLATFORM //////////////////////////////////////////////////////////
		UC_ASSERT_MSG(io.BackendPlatformUserData == nullptr, "Already initialized a platform backend!");
		io.BackendPlatformUserData = this;
		io.BackendPlatformName = "unicore";
		//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//io.SetClipboardTextFn = ;
		//io.GetClipboardTextFn = ;
		io.ClipboardUserData = nullptr;

		// BACKEND RENDER //////////////////////////////////////////////////////////
		UC_ASSERT_MSG(io.BackendRendererUserData == nullptr, "Already initialized a renderer backend!");
		//io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

		io.BackendRendererUserData = this;
		io.BackendRendererName = "unicore_render";
		//io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	}

	ImGuiContext::~ImGuiContext()
	{
		ImGui::DestroyContext(_handle);
	}

	void ImGuiContext::frame_begin()
	{
		//ImGui::SetCurrentContext(_handle);

		// UPDATE INPUT

		auto& io = ImGui::GetIO();

		// MOUSE
		{
			auto& mouse_pos = _input.mouse().position();
			auto& mouse_delta = _input.mouse().delta();
			auto& mouse_wheel = _input.mouse().wheel();

			if (mouse_delta.length_squared() > 0)
				io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);

			io.AddMouseWheelEvent(mouse_wheel.x, mouse_wheel.y);

			io.AddMouseButtonEvent(0, _input.mouse().down(MouseButton::Left));
			io.AddMouseButtonEvent(1, _input.mouse().down(MouseButton::Right));
			io.AddMouseButtonEvent(2, _input.mouse().down(MouseButton::Middle));
		}

		io.DeltaTime = static_cast<float>(_time.delta().total_seconds());

		_render.begin_frame(io);

		ImGui::NewFrame();
	}

	void ImGuiContext::frame_end()
	{
		//ImGui::SetCurrentContext(_handle);
		ImGui::Render();
	}

	void ImGuiContext::render() const
	{
		ImGui::SetCurrentContext(_handle);
		const auto data = ImGui::GetDrawData();
		_render.render(data);
	}
}