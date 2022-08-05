#pragma once
#include "unicore/Buffer2.hpp"
#include "unicore/GridTopology.hpp"

namespace unicore
{
	template<typename TCell>
	class GridMap : public Buffer2<TCell>
	{
	public:
		GridTopology& topology;

		GridMap(const Vector2i& size, GridTopology& topology)
			: Buffer2<TCell>(size),
			topology(topology)
		{
		}

		GridMap(int x, int y, GridTopology& topology)
			: Buffer2<TCell>(x, y),
			topology(topology)
		{
		}
	};
}