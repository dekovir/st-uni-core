#pragma once
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	enum class KeyCode : uint16_t
	{
		BackSpace,
		Tab,
		Return,
		Escape,
		Space,
		Home,
		End,
		PageUp,
		PageDown,
		ArrowLeft,
		ArrowUp,
		ArrowRight,
		ArrowDown,
		Insert,
		Delete,
		Colon,
		Semicolon,

		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Multiply,
		Add,
		Separator,
		Subtract,
		Decimal,
		Divide,

		WinLeft,
		WinRight,

		// NumPad
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		NumPadMultiply,
		NumPadAdd,
		NumPadSeparator,
		NumPadSubtract,
		NumPadDecimal,
		NumPadDivide,
		NumPadEnter,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		ShiftLeft,
		ShiftRight,
		ControlLeft,
		ControlRight,
		AltLeft,
		AltRight,

		BracketLeft,   // [
		BracketRight,  // ]

		Equal,          /* = */
		Slash,          /* / */
		BackSlash,      /* \ */
		Minus,
		Comma,          /* , */
		Period,         /* . */
		Apostrophe,     /* ' */
		GraveAccent,   /* ` */

		// Audio
		AudioPlay,
		AudioStop,
		AudioNext,
		AudioPrev,

		MaxKeyCode,

		// Synonyms
		AsciiBegin = Num0,
		AsciiEnd = Z,
		Enter = Return,
	};

	enum class MouseButton
	{
		Left,
		Middle,
		Right,
	};

	class Input : public Module
	{
	public:
		UC_NODISCARD virtual bool mouse_button(uint8_t button) const = 0;
		UC_NODISCARD virtual const Vector2i& mouse_position() const = 0;

		UC_NODISCARD virtual bool mouse_button(MouseButton button) const
		{
			return mouse_button(static_cast<uint8_t>(button));
		}

		UC_NODISCARD virtual bool keyboard(KeyCode code) const = 0;
	};
}