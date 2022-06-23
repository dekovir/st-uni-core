#pragma once
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Render : public Module
	{
	public:
		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;
	};
}
