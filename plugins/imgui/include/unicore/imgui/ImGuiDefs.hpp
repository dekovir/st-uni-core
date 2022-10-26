#pragma once
#include "unicore/math/Rect.hpp"
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
		static Vector2f convert(const ImVec2& vec)
		{
			return { vec.x, vec.y };
		}

		static ImVec2 convert(const Vector2f& vec)
		{
			return { vec.x, vec.y };
		}
	};
}