#pragma once
#include "unicore/system/Object.hpp"

namespace unicore
{
	enum class InputDeviceType
	{
		Mouse,
		Keyboard,
	};

	enum class ButtonState
	{
		Up, // button is held down
		Down, // button is held released
		UpChanged, // button has been released on this frame
		DownChanged, // button has been pressed on this frame
	};

	class InputDevice : public Object
	{
		UC_OBJECT(InputDevice, Object)
	public:
		UC_NODISCARD virtual InputDeviceType device_type() const = 0;

	protected:
		static ButtonState get_state(bool state_prev, bool state_cur)
		{
			if (state_prev == state_cur)
				return state_cur ? ButtonState::Down : ButtonState::Up;

			if (state_prev)
				return ButtonState::UpChanged;

			return ButtonState::DownChanged;
		}
	};

	template<typename DeviceType,
		std::enable_if_t<std::is_base_of_v<InputDevice, DeviceType>>* = nullptr>
	class InputDeviceState
	{
	public:
		virtual ~InputDeviceState() = default;

		virtual void reset() = 0;
		virtual void update() = 0;

		UC_NODISCARD DeviceType& device() const { return _device; }

	protected:
		DeviceType& _device;

		explicit InputDeviceState(DeviceType& device)
			: _device(device)
		{
		}

		static ButtonState get_state(bool state_prev, bool state_cur)
		{
			if (state_prev == state_cur)
				return state_cur ? ButtonState::Down : ButtonState::Up;

			if (state_prev)
				return ButtonState::UpChanged;

			return ButtonState::DownChanged;
		}
	};
}