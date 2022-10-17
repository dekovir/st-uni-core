#include "unicore/platform/MouseDevice.hpp"

namespace unicore
{
	ButtonState MouseDevice::state(MouseButton button) const
	{
		return state(static_cast<uint8_t>(button));
	}
}