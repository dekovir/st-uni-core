#include "unicore/Input.hpp"

namespace unicore
{
	ButtonState InputDevice::get_state(bool state_prev, bool state_cur)
	{
		if (state_prev == state_cur)
			return state_cur ? ButtonState::Down : ButtonState::Up;

		if (state_prev)
			return ButtonState::UpChanged;

		return ButtonState::DownChanged;
	}

	ButtonState MouseDevice::state(MouseButton button) const
	{
		return state(static_cast<uint8_t>(button));
	}

	bool KeyboardDevice::mods(KeyModCombine mod) const
	{
		const auto flags = mods();
		switch (mod)
		{
		case KeyModCombine::System:
			return flags.any<KeyMod::SystemLeft, KeyMod::SystemRight>();

		case KeyModCombine::Shift:
			return flags.any<KeyMod::ShiftLeft, KeyMod::ShiftRight>();

		case KeyModCombine::Control:
			return flags.any<KeyMod::ControlLeft, KeyMod::ControlRight>();

		case KeyModCombine::Alt:
			return flags.any<KeyMod::AltLeft, KeyMod::AltRight>();
		}

		UC_ASSERT_ALWAYS_MSG("Unknown flag");
		return false;
	}
}