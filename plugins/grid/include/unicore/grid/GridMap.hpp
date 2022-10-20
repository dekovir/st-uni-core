#pragma once
#include "unicore/system/Buffer2.hpp"
#include "unicore/grid/GridTopology.hpp"

namespace unicore
{
	template<typename TCell>
	class GridMap : public Buffer2<TCell>
	{
	public:
		GridTopology& topology;

		GridMap(const Vector2i& size, GridTopology& topology)
			: Buffer2<TCell>(size)
			, topology(topology)
		{
		}

		GridMap(int x, int y, GridTopology& topology)
			: Buffer2<TCell>(x, y)
			, topology(topology)
		{
		}
	};
}