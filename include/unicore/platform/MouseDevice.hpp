#pragma once
#include "unicore/platform/InputDevice.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	enum class MouseButton
	{
		Button0,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,

		MaxButton,

		Left = Button0,
		Middle = Button1,
		Right = Button2,
	};

	class MouseDevice : public InputDevice
	{
		UC_OBJECT(MouseDevice, InputDevice)
	public:
		UC_NODISCARD InputDeviceType device_type() const override { return InputDeviceType::Mouse; }

		UC_NODISCARD virtual Bool button(uint8_t button) const = 0;
		UC_NODISCARD virtual const Vector2i& position() const = 0;
		UC_NODISCARD virtual const Vector2i& wheel() const = 0;
	};

	class MouseDeviceState : public InputDeviceState<MouseDevice>
	{
	public:
		static constexpr size_t ButtonCount = static_cast<size_t>(MouseButton::MaxButton);

		explicit MouseDeviceState(MouseDevice& device);

		void reset() override;
		void update() override;

		UC_NODISCARD const Vector2i& position() const { return _current.position; }
		UC_NODISCARD const Vector2i& wheel() const { return _current.wheel; };
		UC_NODISCARD const Vector2i& delta() const { return _delta; }

		UC_NODISCARD ButtonState state(uint8_t button) const;
		UC_NODISCARD ButtonState state(MouseButton button = MouseButton::Left) const;

		UC_NODISCARD bool up(uint8_t button) const { return state(button) == ButtonState::Up; }
		UC_NODISCARD bool down(uint8_t button) const { return state(button) == ButtonState::Down; }
		UC_NODISCARD bool up_changed(uint8_t button) const { return state(button) == ButtonState::UpChanged; }
		UC_NODISCARD bool down_changed(uint8_t button) const { return state(button) == ButtonState::DownChanged; }

		UC_NODISCARD bool up(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::Up; }
		UC_NODISCARD bool down(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::Down; }
		UC_NODISCARD bool up_changed(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::UpChanged; }
		UC_NODISCARD bool down_changed(MouseButton button = MouseButton::Left) const { return state(button) == ButtonState::DownChanged; }

	protected:
		struct State
		{
			Vector2i position;
			Vector2i wheel;
			Bitset<ButtonCount> buttons;
		};

		State _prev, _current;
		Vector2i _delta;
	};
}