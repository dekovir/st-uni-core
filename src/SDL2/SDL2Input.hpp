#pragma once
#include "unicore/Input.hpp"
#if defined(UNICORE_USE_SDL2)
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

		UC_NODISCARD bool keyboard(KeyCode code) const override;

		void reset();

		void apply_event(const SDL_MouseButtonEvent& evt);
		void apply_event(const SDL_MouseMotionEvent& evt);
		void apply_event(const SDL_KeyboardEvent& evt);

	protected:
		Logger& _logger;
		Array<bool, 3> _mouse_button = { false };
		Vector2i _mouse_pos = Vector2i::Zero;

		Array<bool, static_cast<size_t>(KeyCode::MaxKeyCode)> _keys = { false };
	};
}
#endif