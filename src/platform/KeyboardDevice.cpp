#include "unicore/platform/KeyboardDevice.hpp"

namespace unicore
{
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