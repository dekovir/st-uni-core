#include "unicore/input/KeyboardDevice.hpp"

namespace unicore
{
	KeyboardDeviceState::KeyboardDeviceState(KeyboardDevice& device)
		: InputDeviceState<KeyboardDevice>(device)
		, _prev({}), _current({})
	{
	}

	void KeyboardDeviceState::reset()
	{
		_prev = {};
		_current = {};
	}

	void KeyboardDeviceState::update()
	{
		_prev = _current;

		_current.mods = _device.mods();
		for (size_t i = 0; i < KeyCount; i++)
			_current.keys[i] = _device.key(static_cast<KeyCode>(i));
	}

	KeyModFlags KeyboardDeviceState::mods() const
	{
		return _current.mods;
	}

	bool KeyboardDeviceState::mods(KeyModCombine mod) const
	{
		switch (mod)
		{
		case KeyModCombine::System:
			return _current.mods.any<KeyMod::SystemLeft, KeyMod::SystemRight>();

		case KeyModCombine::Shift:
			return _current.mods.any<KeyMod::ShiftLeft, KeyMod::ShiftRight>();

		case KeyModCombine::Control:
			return _current.mods.any<KeyMod::ControlLeft, KeyMod::ControlRight>();

		case KeyModCombine::Alt:
			return _current.mods.any<KeyMod::AltLeft, KeyMod::AltRight>();
		}

		UC_ASSERT_ALWAYS_MSG("Unknown flag");
		return false;
	}

	ButtonState KeyboardDeviceState::state(KeyCode code) const
	{
		const auto index = static_cast<unsigned>(code);
		return get_state(_prev.keys[index], _current.keys[index]);
	}
}