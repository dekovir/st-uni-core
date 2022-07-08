#include "unicore/gridmap/Topology.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	Vector2f RectangleTopology::cell_to_pos(const MapIndex index) const
	{
		// TODO: Implement gap
		return {
			_size.x * static_cast<float>(index.value.x),
			_size.y * static_cast<float>(index.value.y)
		};
	}

	MapIndex RectangleTopology::pos_to_cell(const Vector2f& pos) const
	{
		// TODO: Implement gap
		return {
			Math::ceil(pos.x / _size.x),
			Math::ceil(pos.y / _size.y)
		};
	}
}