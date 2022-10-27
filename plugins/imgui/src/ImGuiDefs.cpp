#include "unicore/imgui/ImGuiDefs.hpp"

namespace unicore
{
	static const Dictionary<KeyCode, ImGuiKey> s_keys =
	{
		{KeyCode::Tab, ImGuiKey_Tab},
		{KeyCode::ArrowLeft, ImGuiKey_LeftArrow},
		{KeyCode::ArrowRight, ImGuiKey_RightArrow},
		{KeyCode::ArrowUp, ImGuiKey_UpArrow},
		{KeyCode::ArrowDown, ImGuiKey_DownArrow},

		{KeyCode::PageUp, ImGuiKey_PageUp},
		{KeyCode::PageDown, ImGuiKey_PageDown},
		{KeyCode::Home, ImGuiKey_Home},
		{KeyCode::End, ImGuiKey_End},
		{KeyCode::Insert, ImGuiKey_Insert},
		{KeyCode::Delete, ImGuiKey_Delete},
		{KeyCode::BackSpace, ImGuiKey_Backspace},
		{KeyCode::Space, ImGuiKey_Space},
		{KeyCode::Enter, ImGuiKey_Enter},
		{KeyCode::Escape, ImGuiKey_Escape},

		{KeyCode::ControlLeft, ImGuiKey_LeftCtrl},
		{KeyCode::ShiftLeft, ImGuiKey_LeftShift},
		{KeyCode::AltLeft, ImGuiKey_LeftAlt},
		{KeyCode::SystemLeft, ImGuiKey_LeftSuper},
		{KeyCode::ControlRight, ImGuiKey_RightCtrl},
		{KeyCode::ShiftRight, ImGuiKey_RightShift},
		{KeyCode::AltRight, ImGuiKey_RightAlt},
		{KeyCode::SystemRight, ImGuiKey_RightSuper},

		{KeyCode::Num0, ImGuiKey_0},
		{KeyCode::Num1, ImGuiKey_1}, {KeyCode::Num2, ImGuiKey_2}, {KeyCode::Num3, ImGuiKey_3},
		{KeyCode::Num4, ImGuiKey_4}, {KeyCode::Num5, ImGuiKey_5}, {KeyCode::Num6, ImGuiKey_6},
		{KeyCode::Num7, ImGuiKey_7}, {KeyCode::Num8, ImGuiKey_8}, {KeyCode::Num9, ImGuiKey_9},

		{KeyCode::A, ImGuiKey_A}, {KeyCode::B, ImGuiKey_B}, {KeyCode::C, ImGuiKey_C},
		{KeyCode::D, ImGuiKey_D}, {KeyCode::E, ImGuiKey_E}, {KeyCode::F, ImGuiKey_F},
		{KeyCode::G, ImGuiKey_G}, {KeyCode::H, ImGuiKey_H}, {KeyCode::I, ImGuiKey_I},
		{KeyCode::J, ImGuiKey_J}, {KeyCode::K, ImGuiKey_K}, {KeyCode::L, ImGuiKey_L},
		{KeyCode::M, ImGuiKey_M}, {KeyCode::N, ImGuiKey_N}, {KeyCode::O, ImGuiKey_O},
		{KeyCode::P, ImGuiKey_P}, {KeyCode::Q, ImGuiKey_Q}, {KeyCode::R, ImGuiKey_R},
		{KeyCode::S, ImGuiKey_S}, {KeyCode::T, ImGuiKey_T}, {KeyCode::U, ImGuiKey_U},
		{KeyCode::V, ImGuiKey_V}, {KeyCode::W, ImGuiKey_W}, {KeyCode::X, ImGuiKey_X},
		{KeyCode::Y, ImGuiKey_Y}, {KeyCode::Z, ImGuiKey_Z},

		{KeyCode::F1, ImGuiKey_F1}, {KeyCode::F2, ImGuiKey_F2}, {KeyCode::F3, ImGuiKey_F3},
		{KeyCode::F4, ImGuiKey_F4}, {KeyCode::F5, ImGuiKey_F5}, {KeyCode::F6, ImGuiKey_F6},
		{KeyCode::F7, ImGuiKey_F7}, {KeyCode::F8, ImGuiKey_F8}, {KeyCode::F9, ImGuiKey_F9},
		{KeyCode::F10, ImGuiKey_F10}, {KeyCode::F11, ImGuiKey_F11}, {KeyCode::F12, ImGuiKey_F12},

		{KeyCode::Apostrophe, ImGuiKey_Apostrophe},
		{KeyCode::Comma, ImGuiKey_Comma},
		{KeyCode::Minus, ImGuiKey_Minus},
		{KeyCode::Period, ImGuiKey_Period},
		{KeyCode::Slash, ImGuiKey_Slash},
		{KeyCode::Semicolon, ImGuiKey_Semicolon},
		{KeyCode::Equal, ImGuiKey_Equal},
		{KeyCode::BracketLeft, ImGuiKey_LeftBracket},
		{KeyCode::BackSlash, ImGuiKey_Backslash},
		{KeyCode::BracketRight, ImGuiKey_RightBracket},
		{KeyCode::GraveAccent, ImGuiKey_GraveAccent},

		{KeyCode::NumPad0, ImGuiKey_Keypad0},
		{KeyCode::NumPad1, ImGuiKey_Keypad1}, {KeyCode::NumPad2, ImGuiKey_Keypad2}, {KeyCode::NumPad3, ImGuiKey_Keypad3},
		{KeyCode::NumPad4, ImGuiKey_Keypad4}, {KeyCode::NumPad5, ImGuiKey_Keypad5}, {KeyCode::NumPad6, ImGuiKey_Keypad6},
		{KeyCode::NumPad7, ImGuiKey_Keypad7}, {KeyCode::NumPad8, ImGuiKey_Keypad8}, {KeyCode::NumPad9, ImGuiKey_Keypad9},
		{KeyCode::NumPadDecimal, ImGuiKey_KeypadDecimal},
		{KeyCode::NumPadDivide, ImGuiKey_KeypadDivide},
		{KeyCode::NumPadMultiply, ImGuiKey_KeypadMultiply},
		{KeyCode::NumPadSubtract, ImGuiKey_KeypadSubtract},
		{KeyCode::NumPadAdd, ImGuiKey_KeypadAdd},
		{KeyCode::NumPadEnter, ImGuiKey_KeypadEnter},
		//{KeyCode::NumPad, ImGuiKey_KeypadEqual},
	};

	Optional<ImGuiKey> ImGuiConvert::convert(KeyCode code)
	{
		if (const auto it = s_keys.find(code); it != s_keys.end())
			return it->second;

		return std::nullopt;
	}
}