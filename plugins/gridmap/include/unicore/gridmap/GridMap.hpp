#pragma once
#include "unicore/Buffer2.hpp"
#include "unicore/gridmap/Topology.hpp"

namespace unicore
{
	template<typename TCell>
	class GridMap : public Buffer2<TCell>
	{
	public:
		Topology& topology;

		GridMap(const Vector2i& size, Topology& topology)
			: Buffer2<TCell>(size),
			topology(topology)
		{
		}

		GridMap(int x, int y, Topology& topology)
			: Buffer2<TCell>(x, y),
			topology(topology)
		{
		}
	};
}