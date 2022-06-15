#pragma once
#include "unicore/Module.hpp"
#include "unicore/TimeSpan.hpp"

namespace unicore
{
	class Time : public Module
	{
	public:
		virtual const TimeSpan& elapsed() const = 0;
		virtual const TimeSpan& delta() const = 0;
	};
}