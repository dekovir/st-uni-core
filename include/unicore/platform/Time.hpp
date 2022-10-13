#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/system/TimeSpan.hpp"

namespace unicore
{
	class Time : public Object
	{
		UC_OBJECT(Time, Object)
	public:
		UC_NODISCARD virtual const TimeSpan& elapsed() const = 0;
		UC_NODISCARD virtual const TimeSpan& delta() const = 0;
	};
}