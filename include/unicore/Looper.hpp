#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class Looper : public Object
	{
	public:
		UC_NODISCARD virtual bool running() const = 0;
		virtual void quit() = 0;

		virtual void poll_events() = 0;
	};
}