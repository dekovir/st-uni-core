#include "unicore/Input.hpp"

namespace unicore
{
	bool Input::mouse_button(MouseButton button) const
	{
		return mouse_button(static_cast<uint8_t>(button));
	}

	bool Input::key_mod(KeyModCombine mod) const
	{
		const auto flags = key_mod();
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