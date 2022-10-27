#pragma once
#include "unicore/platform/Input.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class Logger;

	class SDL2InputDevice : public SDL2EventListener
	{
	public:
		explicit SDL2InputDevice(Logger& logger)
			: _logger(logger)
		{
		}

		virtual ~SDL2InputDevice() = default;

		virtual void reset() = 0;
		virtual void frame() = 0;

	protected:
		Logger& _logger;
	};

	class SDL2MouseDevice : public MouseDevice, public SDL2InputDevice
	{
		UC_OBJECT(SDL2MouseDevice, MouseDevice)
	public:
		explicit SDL2MouseDevice(Logger& logger);

		UC_NODISCARD Bool button(uint8_t button) const override;

		UC_NODISCARD const Vector2i& position() const override { return _position; }
		UC_NODISCARD const Vector2i& wheel() const override { return _wheel; }

		void reset() override;
		void frame() override;

		bool on_event(const SDL_Event& evt) override;

	protected:
		Bitset<3> _buttons = { false };
		Vector2i _position = VectorConst2i::Zero;
		Vector2i _wheel = VectorConst2i::Zero;
	};

	class SDL2KeyboardDevice : public KeyboardDevice, public SDL2InputDevice
	{
		UC_OBJECT(SDL2KeyboardDevice, KeyboardDevice)
	public:
		explicit SDL2KeyboardDevice(Logger& logger);

		UC_NODISCARD Bool key(KeyCode code) const override;
		UC_NODISCARD KeyModFlags mods() const override;

		UC_NODISCARD const String32& text() const override;

		void reset() override;
		void frame() override;

		bool on_event(const SDL_Event& evt) override;

	protected:
		Bitset<static_cast<size_t>(KeyCode::MaxKeyCode)> _state = { false };
		KeyModFlags _mods;
		String32 _text;

		static void update_flags(const SDL_Keymod mod, KeyModFlags& flags);
	};

	class SDL2TouchDevice : public TouchDevice, SDL2InputDevice
	{
	public:
		explicit SDL2TouchDevice(Logger& logger);

		void reset() override;
		void frame() override;

		bool on_event(const SDL_Event& evt) override;

		UC_NODISCARD const List<TouchFinger>& fingers() const override { return _fingers; }

	protected:
		List<TouchFinger> _fingers;
	};

	class SDL2Input : public Input, public SDL2EventListener
	{
		UC_OBJECT(SDL2Input, Input)
	public:
		explicit SDL2Input(Logger& logger);

		UC_NODISCARD const MouseDeviceState& mouse() const override;
		UC_NODISCARD const KeyboardDeviceState& keyboard() const override;
		UC_NODISCARD const TouchDeviceState& touch() const override;

		void reset();
		void frame();
		void update();

		bool on_event(const SDL_Event& evt) override;

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