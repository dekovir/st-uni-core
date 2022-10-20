#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/io/Path.hpp"

namespace unicore
{
	class Looper : public Object
	{
		UC_OBJECT_EVENT(drop_file, const Path&);
	public:
		UC_NODISCARD virtual bool running() const = 0;
		virtual void quit() = 0;

		virtual void poll_events() = 0;
	};
}