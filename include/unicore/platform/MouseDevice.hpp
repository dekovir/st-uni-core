#pragma once
#include "unicore/platform/InputDevice.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
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

	class MouseDevice : public InputDevice
	{
		UC_OBJECT(MouseDevice, InputDevice)
	public:
		UC_NODISCARD InputDeviceType device_type() const override { return InputDeviceType::Mouse; }

		UC_NODISCARD virtual ButtonState state(uint8_t button) const = 0;
		UC_NODISCARD virtual ButtonState state(MouseButton button = MouseButton::Left) const;

		UC_NODISCARD bool up(uint8_t button) const { return state(button) == ButtonState::Up; }
		UC_NODISCARD bool down(uint8_t button) const { return state(button) == ButtonState::Down; }
		UC_NODISCARD bool up_changed(uint8_t button) const { return state(button) == ButtonState::UpChanged; }
		UC_NODISCARD bool down_changed(uint8_t button) const { return state(button) == ButtonState::DownChanged; }

		UC_NODISCARD bool up(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::Up; }
		UC_NODISCARD bool down(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::Down; }
		UC_NODISCARD bool up_changed(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::UpChanged; }
		UC_NODISCARD bool down_changed(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::DownChanged; }

		UC_NODISCARD virtual const Vector2i& position() const = 0;
		UC_NODISCARD virtual const Vector2i& delta() const = 0;
		UC_NODISCARD virtual const Vector2i& wheel() const = 0;
	};
}