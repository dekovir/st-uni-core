#pragma once
#include "unicore/math/Rect.hpp"
#include "unicore/input/Keyboard.hpp"
#include "unicore/renderer/Color4.hpp"
#include <imgui.h>

namespace ImGui
{
	using Context = ImGuiContext;
	using IO = ImGuiIO;
}

namespace unicore
{
	class ImGuiConvert
	{
	public:
		static constexpr Vector2f convert(const ImVec2& vec) { return { vec.x, vec.y }; }
		static constexpr ImVec2 convert(const Vector2f& vec) { return { vec.x, vec.y }; }

		static constexpr ImColor convert_color(const Color4f& v) { return { v.r, v.g, v.b, v.a }; }
		static constexpr Color4f convert_color(const ImColor& v) { return { v.Value.x, v.Value.y, v.Value.z, v.Value.w }; }

		static Optional<ImGuiKey> convert(KeyCode code);
	};
}