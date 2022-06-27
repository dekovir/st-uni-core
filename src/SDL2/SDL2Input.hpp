#pragma once
#include "unicore/Input.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Input : public Input
	{
	public:
		SDL2Input();

		UC_NODISCARD bool mouse_button(uint8_t button) const override;
		UC_NODISCARD const Vector2i& mouse_position() const override;

		void apply_event(const SDL_MouseButtonEvent& evt);
		void apply_event(const SDL_MouseMotionEvent& evt);

	protected:
		Array<bool, 3> _mouseBtn = { false };
		Vector2i _mousePos = Vector2i::Zero;
	};
}
#endif