#pragma once
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Input : public Module
	{
	public:
		virtual bool mouse_button(int button) const = 0;
		virtual const Vector2i& mouse_position() const = 0;
	};
}