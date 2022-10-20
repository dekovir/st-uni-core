#pragma once
#include "unicore/input/InputDevice.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	struct TouchFinger
	{
		IntPtr id;
		Vector2i position;
	};

	class TouchDevice : public InputDevice
	{
	public:
		UC_NODISCARD InputDeviceType device_type() const override { return InputDeviceType::Touch; }

		UC_NODISCARD virtual const List<TouchFinger>& fingers() const = 0;
	};

	class TouchDeviceState : public InputDeviceState<TouchDevice>
	{
	public:
		explicit TouchDeviceState(TouchDevice& device);

		UC_NODISCARD const List<TouchFinger>& fingers() const;

		void reset() override;
		void update() override;

	protected:
		List<TouchFinger> _prev, _current;
	};
}