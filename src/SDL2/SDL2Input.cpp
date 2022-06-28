#include "SDL2Input.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Logger.hpp"

namespace unicore
{
	// https://wiki.libsdl.org/SDL_Scancode
	const std::map<SDL_Keycode, KeyCode> s_key_bindings =
	{
			{SDLK_SPACE, KeyCode::Space},
			{SDLK_0, KeyCode::Num0},
			{SDLK_1, KeyCode::Num1},
			{SDLK_2, KeyCode::Num2},
			{SDLK_3, KeyCode::Num3},
			{SDLK_4, KeyCode::Num4},
			{SDLK_5, KeyCode::Num5},
			{SDLK_6, KeyCode::Num6},
			{SDLK_7, KeyCode::Num7},
			{SDLK_8, KeyCode::Num8},
			{SDLK_9, KeyCode::Num9},
			{SDLK_SEMICOLON, KeyCode::Semicolon},
			{SDLK_a, KeyCode::A},
			{SDLK_b, KeyCode::B},
			{SDLK_c, KeyCode::C},
			{SDLK_d, KeyCode::D},
			{SDLK_e, KeyCode::E},
			{SDLK_f, KeyCode::F},
			{SDLK_g, KeyCode::G},
			{SDLK_h, KeyCode::H},
			{SDLK_i, KeyCode::I},
			{SDLK_j, KeyCode::K},
			{SDLK_k, KeyCode::K},
			{SDLK_l, KeyCode::L},
			{SDLK_m, KeyCode::M},
			{SDLK_n, KeyCode::N},
			{SDLK_o, KeyCode::O},
			{SDLK_p, KeyCode::P},
			{SDLK_q, KeyCode::Q},
			{SDLK_r, KeyCode::R},
			{SDLK_s, KeyCode::S},
			{SDLK_t, KeyCode::T},
			{SDLK_u, KeyCode::U},
			{SDLK_v, KeyCode::V},
			{SDLK_w, KeyCode::W},
			{SDLK_x, KeyCode::X},
			{SDLK_y, KeyCode::Y},
			{SDLK_z, KeyCode::Z},
			{SDLK_LEFTBRACKET, KeyCode::BracketLeft},
			{SDLK_RIGHTBRACKET, KeyCode::BracketRight},
			{SDLK_ESCAPE, KeyCode::Escape},
			{SDLK_RETURN, KeyCode::Return},
			{SDLK_TAB, KeyCode::Tab},
			{SDLK_BACKSPACE, KeyCode::BackSpace},
			{SDLK_INSERT, KeyCode::Insert},
			{SDLK_DELETE, KeyCode::Delete},
			{SDLK_RIGHT, KeyCode::ArrowRight},
			{SDLK_LEFT, KeyCode::ArrowLeft},
			{SDLK_DOWN, KeyCode::ArrowDown},
			{SDLK_UP, KeyCode::ArrowUp},
			{SDLK_PAGEUP, KeyCode::PageUp},
			{SDLK_PAGEDOWN, KeyCode::PageDown},
			{SDLK_HOME, KeyCode::Home},
			{SDLK_END, KeyCode::End},

			// F KEYS
			{SDLK_F1, KeyCode::F1},
			{SDLK_F2, KeyCode::F2},
			{SDLK_F3, KeyCode::F3},
			{SDLK_F4, KeyCode::F4},
			{SDLK_F5, KeyCode::F5},
			{SDLK_F6, KeyCode::F6},
			{SDLK_F7, KeyCode::F7},
			{SDLK_F8, KeyCode::F8},
			{SDLK_F9, KeyCode::F9},
			{SDLK_F10, KeyCode::F10},
			{SDLK_F11, KeyCode::F11},
			{SDLK_F12, KeyCode::F12},

			// KEYPAD / NUMPAD
			{SDLK_KP_0, KeyCode::NumPad0},
			{SDLK_KP_1, KeyCode::NumPad1},
			{SDLK_KP_2, KeyCode::NumPad2},
			{SDLK_KP_3, KeyCode::NumPad3},
			{SDLK_KP_4, KeyCode::NumPad4},
			{SDLK_KP_5, KeyCode::NumPad5},
			{SDLK_KP_6, KeyCode::NumPad6},
			{SDLK_KP_7, KeyCode::NumPad7},
			{SDLK_KP_8, KeyCode::NumPad8},
			{SDLK_KP_9, KeyCode::NumPad9},
			{SDLK_KP_DECIMAL, KeyCode::NumPadDecimal},
			{SDLK_KP_DIVIDE, KeyCode::NumPadDivide},
			{SDLK_KP_MULTIPLY, KeyCode::NumPadMultiply},
			{SDLK_KP_MINUS, KeyCode::NumPadSubtract},
			{SDLK_KP_PLUS, KeyCode::NumPadAdd},
			{SDLK_KP_ENTER, KeyCode::NumPadEnter},

			// AUDIO
			{ SDLK_AUDIOPLAY, KeyCode::AudioPlay },
			{ SDLK_AUDIOSTOP, KeyCode::AudioStop },
			{ SDLK_AUDIONEXT, KeyCode::AudioNext },
			{ SDLK_AUDIOPREV, KeyCode::AudioPrev },

			// OTHER
			{SDLK_SLASH, KeyCode::Slash},
			{SDLK_BACKSLASH, KeyCode::BackSlash},
			{SDLK_QUOTE, KeyCode::Apostrophe},
			{SDLK_COMMA, KeyCode::Comma},
			{SDLK_PERIOD, KeyCode::Period},
			{SDLK_MINUS, KeyCode::Minus},
			{SDLK_EQUALS, KeyCode::Equal},
			{SDLK_BACKQUOTE, KeyCode::GraveAccent},

			// MODIFIERS
			{SDLK_LSHIFT, KeyCode::ShiftLeft},
			{SDLK_LCTRL, KeyCode::ControlLeft},
			{SDLK_LALT, KeyCode::AltLeft},
			{SDLK_LGUI, KeyCode::WinLeft},
			{SDLK_RSHIFT, KeyCode::ShiftRight},
			{SDLK_RCTRL, KeyCode::ControlRight},
			{SDLK_RALT, KeyCode::AltRight},
			{SDLK_RGUI, KeyCode::WinRight},
	};

	SDL2Input::SDL2Input(Logger& logger)
		: _logger(logger)
	{
		SDL_InitSubSystem(SDL_INIT_EVENTS);
	}

	bool SDL2Input::mouse_button(uint8_t button) const
	{
		return button < _mouse_button.size() && _mouse_button[button];
	}

	const Vector2i& SDL2Input::mouse_position() const
	{
		return _mouse_pos;
	}

	bool SDL2Input::keyboard(KeyCode code) const
	{
		return _keys[static_cast<int>(code)];
	}

	void SDL2Input::reset()
	{
		_mouse_button.fill(false);
		_mouse_pos = Vector2iConst::Zero;

		_keys.fill(false);
	}

	void SDL2Input::apply_event(const SDL_MouseButtonEvent& evt)
	{
		if (evt.button >= 1 && evt.button <= _mouse_button.size())
		{
			_mouse_button[evt.button - 1] = evt.type == SDL_MOUSEBUTTONDOWN;
			_mouse_pos.x = evt.x;
			_mouse_pos.y = evt.y;
		}
	}

	void SDL2Input::apply_event(const SDL_MouseMotionEvent& evt)
	{
		_mouse_pos.x = evt.x;
		_mouse_pos.y = evt.y;
	}

	void SDL2Input::apply_event(const SDL_KeyboardEvent& evt)
	{
		const auto it = s_key_bindings.find(evt.keysym.sym);
		if (it != s_key_bindings.end())
		{
			_keys[static_cast<int>(it->second)] = evt.state == SDL_PRESSED;
		}
		else
		{
			UC_LOG_WARNING(_logger) << "Unknown key " << evt.keysym.sym;
		}
	}
}
#endif