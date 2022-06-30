#pragma once
#include "unicore/Input.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Input.hpp"
#include <SDL.h>

namespace unicore
{
	class Logger;

	class SDL2Input : public Input
	{
	public:
		explicit SDL2Input(Logger& logger);

		UC_NODISCARD bool mouse_button(uint8_t button) const override;
		UC_NODISCARD const Vector2i& mouse_position() const override;

		UC_NODISCARD bool key_code(KeyCode code) const override;
		UC_NODISCARD KeyModFlags key_mod() const override;

		void reset();
		void update();

	protected:
		Logger& _logger;
		Bitset<3> _mouse_button = { false };
		Vector2i _mouse_pos = VectorConst2i::Zero;

		Bitset<static_cast<size_t>(KeyCode::MaxKeyCode)> _key_code = { false };
		KeyModFlags _key_mod;
	};
}
#endif