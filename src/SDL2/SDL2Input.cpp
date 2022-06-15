#include "SDL2Input.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	bool SDL2Input::mouse_button(int button) const
	{
		return button >= 0 && button < _mouseBtn.size() && _mouseBtn[button];
	}

	const Vector2i& SDL2Input::mouse_position() const
	{
		return _mousePos;
	}

	void SDL2Input::ApplyEvent(const SDL_MouseButtonEvent& evt)
	{
		if (evt.button >= 1 && evt.button <= _mouseBtn.size())
		{
			_mouseBtn[evt.button - 1] = evt.type == SDL_MOUSEBUTTONDOWN;
			_mousePos.x = evt.x;
			_mousePos.y = evt.y;
		}
	}

	void SDL2Input::ApplyEvent(const SDL_MouseMotionEvent& evt)
	{
		_mousePos.x = evt.x;
		_mousePos.y = evt.y;
	}
}
#endif