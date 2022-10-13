#pragma once
#include "unicore/Input.hpp"
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
		UC_NODISCARD ButtonState state(uint8_t button) const override;

		UC_NODISCARD const Vector2i& position() const override { return _position; }
		UC_NODISCARD const Vector2i& delta() const override { return _delta; }
		UC_NODISCARD const Vector2i& wheel() const override { return _wheel; }

		void reset() override;
		void update() override;

	protected:
		Bitset<3> _cur = { false }, _prev = { false };
		Vector2i _position = VectorConst2i::Zero;
		Vector2i _delta = VectorConst2i::Zero;
		Vector2i _wheel = VectorConst2i::Zero;
	};

	class SDL2KeyboardDevice : public KeyboardDevice, public SDL2InputDevice
	{
		UC_OBJECT(SDL2KeyboardDevice, KeyboardDevice)
	public:
		UC_NODISCARD ButtonState state(KeyCode code) const override;
		UC_NODISCARD KeyModFlags mods() const override;

		void reset() override;
		void update() override;

	protected:
		Bitset<static_cast<size_t>(KeyCode::MaxKeyCode)> _prev = { false }, _cur = { false };
		KeyModFlags _key_mod;
	};

	class SDL2Input : public Input
	{
		UC_OBJECT(SDL2Input, Input)
	public:
		explicit SDL2Input(Logger& logger);

		UC_NODISCARD const MouseDevice& mouse() const override { return _mouse; }
		UC_NODISCARD const KeyboardDevice& keyboard() const override { return _keyboard; }

		void reset();
		void update();
	protected:
		Logger& _logger;
		SDL2MouseDevice _mouse;
		SDL2KeyboardDevice _keyboard;
	};
}
#endif