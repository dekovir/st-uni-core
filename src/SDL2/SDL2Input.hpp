#pragma once
#include "unicore/Input.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Input : public Input
	{
	public:
		SDL2Input() = default;

		bool mouse_button(int button) const override;
		const Vector2i& mouse_position() const override;

	protected:
		Array<bool, 3> _mouseBtn = { false };
		Vector2i _mousePos = Vector2i::Zero;

		void ApplyEvent(const SDL_MouseButtonEvent& evt);
		void ApplyEvent(const SDL_MouseMotionEvent& evt);

		friend class SDL2Platform;
	};
}
#endif