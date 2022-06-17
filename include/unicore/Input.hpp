#pragma once
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Input : public Module
	{
	public:
		UC_NODISCARD virtual bool mouse_button(uint8_t button) const = 0;
		UC_NODISCARD virtual const Vector2i& mouse_position() const = 0;
	};
}