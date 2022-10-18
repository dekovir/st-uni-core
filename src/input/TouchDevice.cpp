#include "unicore/input/TouchDevice.hpp"

namespace unicore
{
	TouchDeviceState::TouchDeviceState(TouchDevice& device)
		: InputDeviceState<TouchDevice>(device)
	{
	}

	const List<TouchFinger>& TouchDeviceState::fingers() const
	{
		return _current;
	}

	void TouchDeviceState::reset()
	{
		_prev.clear();
		_current.clear();
	}

	void TouchDeviceState::update()
	{
		_prev = _current;
		_current = _device.fingers();
	}
}