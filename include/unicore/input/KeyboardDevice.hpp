#pragma once
#include "unicore/input/InputDevice.hpp"
#include "unicore/input/Keyboard.hpp"

namespace unicore
{
	class KeyboardDevice : public InputDevice
	{
		UC_OBJECT(KeyboardDevice, InputDevice)
	public:
		UC_NODISCARD InputDeviceType device_type() const override { return InputDeviceType::Keyboard; }

		UC_NODISCARD virtual Bool key(KeyCode code) const = 0;
		UC_NODISCARD virtual KeyModFlags mods() const = 0;

		UC_NODISCARD virtual const String32& text() const = 0;
	};

	class KeyboardDeviceState : public InputDeviceState<KeyboardDevice>
	{
	public:
		static constexpr size_t KeyCount = static_cast<size_t>(KeyCode::MaxKeyCode);

		explicit KeyboardDeviceState(KeyboardDevice& device);

		void reset() override;
		void update() override;

		UC_NODISCARD KeyModFlags mods() const;
		UC_NODISCARD bool mods(KeyModCombine mod) const;
		UC_NODISCARD virtual const String32& text() const;

		UC_NODISCARD ButtonState state(KeyCode code) const;

		UC_NODISCARD bool up(KeyCode code) const { return state(code) == ButtonState::Up; }
		UC_NODISCARD bool down(KeyCode code) const { return state(code) == ButtonState::Down; }
		UC_NODISCARD bool up_changed(KeyCode code) const { return state(code) == ButtonState::UpChanged; }
		UC_NODISCARD bool down_changed(KeyCode code) const { return state(code) == ButtonState::DownChanged; }

	protected:
		struct State
		{
			Bitset<KeyCount> keys;
			KeyModFlags mods;
		};

		State _prev, _current;
		String32 _text;
	};
}