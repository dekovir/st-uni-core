#pragma once
#include "unicore/Object.hpp"
#include "unicore/TimeSpan.hpp"

namespace unicore
{
	class Time : public Object
	{
	public:
		UC_NODISCARD virtual const TimeSpan& elapsed() const = 0;
		UC_NODISCARD virtual const TimeSpan& delta() const = 0;
	};
}