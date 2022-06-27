#include "SDL2Input.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	SDL2Input::SDL2Input()
	{
		SDL_InitSubSystem(SDL_INIT_EVENTS);
	}

	bool SDL2Input::mouse_button(uint8_t button) const
	{
		return button < _mouseBtn.size() && _mouseBtn[button];
	}

	const Vector2i& SDL2Input::mouse_position() const
	{
		return _mousePos;
	}

	void SDL2Input::apply_event(const SDL_MouseButtonEvent& evt)
	{
		if (evt.button >= 1 && evt.button <= _mouseBtn.size())
		{
			_mouseBtn[evt.button - 1] = evt.type == SDL_MOUSEBUTTONDOWN;
			_mousePos.x = evt.x;
			_mousePos.y = evt.y;
		}
	}

	void SDL2Input::apply_event(const SDL_MouseMotionEvent& evt)
	{
		_mousePos.x = evt.x;
		_mousePos.y = evt.y;
	}
}
#endif