#include "unicore/platform/MouseDevice.hpp"

namespace unicore
{
	MouseDeviceState::MouseDeviceState(MouseDevice& device)
		: InputDeviceState<MouseDevice>(device)
		, _prev({}), _current({}), _delta({})
	{
	}

	void MouseDeviceState::reset()
	{
		_prev = {};
		_current = {};
		_delta = {};
	}

	void MouseDeviceState::update()
	{
		_prev = _current;

		_current.position = _device.position();
		_current.wheel = _device.wheel();

		for (uint8_t i = 0; i < ButtonCount; i++)
			_current.buttons[i] = _device.button(i);

		_delta = _current.position - _prev.position;
	}

	ButtonState MouseDeviceState::state(uint8_t button) const
	{
		return get_state(_prev.buttons[button], _current.buttons[button]);
	}

	ButtonState MouseDeviceState::state(MouseButton button) const
	{
		return state(static_cast<uint8_t>(button));
	}
}