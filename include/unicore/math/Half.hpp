#pragma once
#include "unicore/math/Math.hpp"

namespace unicore
{
	// TODO: See implementation at https://github.com/stride3d/stride/blob/master/sources/core/Stride.Core.Mathematics/Half.cs
	class Half
	{
	public:
		constexpr Half() : _value(0) {}
		constexpr explicit Half(UInt16 value) : _value(value) {}

	protected:
		UInt16 _value;
	};
}