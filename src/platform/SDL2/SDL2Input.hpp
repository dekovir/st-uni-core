#pragma once
#include "unicore/platform/Input.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	class Logger;

	class SDL2InputDevice
	{
	public:
		virtual ~SDL2InputDevice() = default;

		virtual void reset() = 0;
		virtual void update() = 0;
	};

	class SDL2MouseDevice : public MouseDevice, public SDL2InputDevice
	{
		UC_OBJECT(SDL2MouseDevice, MouseDevice)
	public:
		UC_NODISCARD Bool button(uint8_t button) const override;

		UC_NODISCARD const Vector2i& position() const override { return _position; }
		UC_NODISCARD const Vector2i& wheel() const override { return _wheel; }

		void reset() override;
		void update() override;

	protected:
		Bitset<3> _buttons = { false };
		Vector2i _position = VectorConst2i::Zero;
		Vector2i _wheel = VectorConst2i::Zero;
	};

	class SDL2KeyboardDevice : public KeyboardDevice, public SDL2InputDevice
	{
		UC_OBJECT(SDL2KeyboardDevice, KeyboardDevice)
	public:
		UC_NODISCARD Bool key(KeyCode code) const override;
		UC_NODISCARD KeyModFlags mods() const override;

		void reset() override;
		void update() override;

	protected:
		Bitset<static_cast<size_t>(KeyCode::MaxKeyCode)> _state = { false };
		KeyModFlags _mods;
	};

	class SDL2TouchDevice : public TouchDevice, SDL2InputDevice
	{
	public:
		void reset() override;
		void update() override;

		UC_NODISCARD const List<TouchFinger>& fingers() const override { return _fingers; }

	protected:
		List<TouchFinger> _fingers;
	};

	class SDL2Input : public Input
	{
		UC_OBJECT(SDL2Input, Input)
	public:
		explicit SDL2Input(Logger& logger);

		UC_NODISCARD const MouseDeviceState& mouse() const override;
		UC_NODISCARD const KeyboardDeviceState& keyboard() const override;
		UC_NODISCARD const TouchDeviceState& touch() const override;

		void reset();
		void update();
	protected:
		Logger& _logger;
		SDL2MouseDevice _mouse;
		SDL2KeyboardDevice _keyboard;
		SDL2TouchDevice _touch;

		MouseDeviceState _mouse_state;
		KeyboardDeviceState _keyboard_state;
		TouchDeviceState _touch_state;
	};
}
#endif