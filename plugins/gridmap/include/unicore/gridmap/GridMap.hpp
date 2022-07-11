#pragma once
#include "unicore/Buffer2.hpp"
#include "unicore/Resource.hpp"
#include "unicore/gridmap/Topology.hpp"

namespace unicore
{
	template<typename TCell>
	class GridMap : public Buffer2<TCell>
	{
	public:
		Topology& topology;

		GridMap(Topology& topology_, int w, int h)
			: Buffer2(w, h)
			, topology(topology_)
		{}

		GridMap(Topology& topology_, const Vector2i& size)
			: Buffer2(size)
			, topology(topology_)
		{}
	};
}