#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	enum class KeyCode : uint8_t
	{
		BackSpace = 1,
		Tab = 2,
		Return = 3,
		Escape = 4,
		Space = 5,

		Home = 20,
		End = 21,
		PageUp = 22,
		PageDown = 23,
		Insert = 24,
		Delete = 25,

		ArrowLeft = 30,
		ArrowUp = 31,
		ArrowRight = 32,
		ArrowDown = 33,

		Colon,
		Semicolon,

		Num0 = 40,
		Num1 = 41,
		Num2 = 42,
		Num3 = 43,
		Num4 = 44,
		Num5 = 45,
		Num6 = 46,
		Num7 = 47,
		Num8 = 48,
		Num9 = 49,

		A = 50,
		B = 51,
		C = 52,
		D = 53,
		E = 54,
		F = 55,
		G = 56,
		H = 57,
		I = 58,
		J = 59,
		K = 60,
		L = 61,
		M = 62,
		N = 63,
		O = 64,
		P = 65,
		Q = 66,
		R = 67,
		S = 68,
		T = 69,
		U = 70,
		V = 71,
		W = 72,
		X = 73,
		Y = 74,
		Z = 75,

		Multiply = 80,
		Add = 81,
		Separator = 82,
		Subtract = 83,
		Decimal = 84,
		Divide = 85,

		// NumPad
		NumPad0 = 100,
		NumPad1 = 101,
		NumPad2 = 102,
		NumPad3 = 103,
		NumPad4 = 104,
		NumPad5 = 105,
		NumPad6 = 106,
		NumPad7 = 107,
		NumPad8 = 108,
		NumPad9 = 109,
		NumPadMultiply = 110,
		NumPadAdd = 111,
		NumPadSeparator = 112,
		NumPadSubtract = 113,
		NumPadDecimal = 114,
		NumPadDivide = 115,
		NumPadEnter = 116,

		F1 = 120,
		F2 = 121,
		F3 = 122,
		F4 = 123,
		F5 = 124,
		F6 = 125,
		F7 = 126,
		F8 = 127,
		F9 = 128,
		F10 = 129,
		F11 = 130,
		F12 = 131,

		SystemLeft = 150,
		SystemRight = 151,
		ShiftLeft = 152,
		ShiftRight = 153,
		ControlLeft = 154,
		ControlRight = 155,
		AltLeft = 156,
		AltRight = 157,

		BracketLeft = 170,
		BracketRight = 171,

		Equal = 180,
		Slash = 181,
		BackSlash = 182,
		Minus = 183,
		Comma = 184,
		Period = 185,
		Apostrophe = 186,
		GraveAccent = 187,

		// Audio
		AudioPlay = 200,
		AudioStop = 201,
		AudioNext = 202,
		AudioPrev = 203,

		MaxKeyCode,

		// Synonyms
		AlphaNumBegin = Num0,
		AlphaNumEnd = Z,
		Enter = Return,
	};

	enum class KeyMod : uint16_t
	{
		SystemLeft = 1 << 0,
		SystemRight = 1 << 1,
		ShiftLeft = 1 << 2,
		ShiftRight = 1 << 3,
		ControlLeft = 1 << 4,
		ControlRight = 1 << 5,
		AltLeft = 1 << 6,
		AltRight = 1 << 7,
	};
	UNICORE_ENUMFLAGS(KeyMod, KeyModFlags);

	enum class KeyModCombine
	{
		System,
		Shift,
		Control,
		Alt,
	};

	enum class MouseButton
	{
		Left,
		Middle,
		Right,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
	};

	class MouseDevice : public Object
	{
	public:
		UC_NODISCARD virtual bool down(uint8_t button) const = 0;
		UC_NODISCARD virtual bool down(MouseButton button) const;

		UC_NODISCARD virtual const Vector2i& position() const = 0;
		UC_NODISCARD virtual const Vector2i& delta() const = 0;
		UC_NODISCARD virtual const Vector2i& wheel() const = 0;
	};

	class KeyboardDevice : public Object
	{
	public:
		UC_NODISCARD virtual bool down(KeyCode code) const = 0;

		UC_NODISCARD virtual KeyModFlags mods() const = 0;
		UC_NODISCARD virtual bool mods(KeyModCombine mod) const;
	};

	class Input : public Module
	{
	public:
		UC_NODISCARD virtual const MouseDevice& mouse() const = 0;
		UC_NODISCARD virtual const KeyboardDevice& keyboard() const = 0;
	};
}