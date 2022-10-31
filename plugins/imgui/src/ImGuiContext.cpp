#include "unicore/imgui/ImGuiContext.hpp"
#include "unicore/system/Memory.hpp"
#include "unicore/imgui/ImGuiDefs.hpp"

namespace unicore
{
	ImGuiContext::ImGuiContext(ImGuiRender& render, Time& time, Input& input, Logger& logger)
		: _render(render), _time(time), _input(input), _logger(logger)
	{
		ImGui::SetAllocatorFunctions(&alloc_func, &free_func, nullptr);
		_handle = ImGui::CreateContext();

		ImGui::SetCurrentContext(_handle);

		ImGui::StyleColorsDark();
		auto& io = ImGui::GetIO();

		io.UserData = this;
		io.IniFilename = nullptr;

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
			const auto mouse_pos = _input.mouse().position().cast<Float>();
			const auto mouse_wheel = _input.mouse().wheel().cast<Float>();

			io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);
			io.AddMouseWheelEvent(mouse_wheel.x, mouse_wheel.y); // return sign?

			io.AddMouseButtonEvent(0, _input.mouse().down(MouseButton::Left));
			io.AddMouseButtonEvent(1, _input.mouse().down(MouseButton::Right));
			io.AddMouseButtonEvent(2, _input.mouse().down(MouseButton::Middle));
		}

		// KEYBOARD
		{
			for (unsigned i = 0; i < static_cast<unsigned>(KeyCode::MaxKeyCode); i++)
			{
				const auto code = static_cast<KeyCode>(i);
				const auto converted = ImGuiConvert::convert(code);
				if (!converted.has_value())
					continue;

				switch (_input.keyboard().state(code))
				{
				case ButtonState::UpChanged:
					io.AddKeyEvent(converted.value(), false);
					break;

				case ButtonState::DownChanged:
					io.AddKeyEvent(converted.value(), true);
					break;

				default:
					break;
				}
			}

			const auto& text = _input.keyboard().text();
			if (!text.empty())
			{
				const auto utf8 = Unicode::to_utf8(text);
				io.AddInputCharactersUTF8(utf8.c_str());
			}

			io.AddKeyEvent(ImGuiKey_ModCtrl,
				_input.keyboard().mods(KeyModCombine::Control));
			io.AddKeyEvent(ImGuiKey_ModShift,
				_input.keyboard().mods(KeyModCombine::Shift));
			io.AddKeyEvent(ImGuiKey_ModAlt,
				_input.keyboard().mods(KeyModCombine::Alt));
			io.AddKeyEvent(ImGuiKey_ModSuper,
				_input.keyboard().mods(KeyModCombine::System));
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

	bool ImGuiContext::is_mouse_over() const
	{
		const auto& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	}

	void* ImGuiContext::alloc_func(size_t sz, void* user_data)
	{
		return Memory::alloc(sz);
	}

	void ImGuiContext::free_func(void* ptr, void* user_data)
	{
		Memory::free(ptr);
	}
}