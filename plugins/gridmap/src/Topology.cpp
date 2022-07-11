#include "unicore/gridmap/Topology.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	Vector2f RectangleTopology::cell_to_pos(const CellIndex index) const
	{
		// TODO: Implement gap
		return {
			(_size.x + _gap.x) * static_cast<float>(index.value.x),
			(_size.y + _gap.y) * static_cast<float>(index.value.y)
		};
	}

	CellIndex RectangleTopology::pos_to_cell(const Vector2f& pos) const
	{
		// TODO: Implement gap
		return {
			Math::ceil(pos.x / _size.x),
			Math::ceil(pos.y / _size.y)
		};
	}

	void RectangleTopology::get_cell_points(const CellIndex index, List<Vector2f>& points) const
	{
		const auto w = _size.x / 2;
		const auto h = _size.y / 2;
		const auto pos = cell_to_pos(index);

		points.push_back({ pos.x - w, pos.y - h });
		points.push_back({ pos.x + w, pos.y - h });
		points.push_back({ pos.x + w, pos.y + h });
		points.push_back({ pos.x - w, pos.y + h });
	}

	CellIndex RectangleTopology::get_cell_neighbor(const CellIndex index, uint8_t dir) const
	{
		switch (dir)
		{
		case static_cast<uint8_t>(RectangleTopologyDir::PositiveX): return { index.value.x + 1, index.value.y };
		case static_cast<uint8_t>(RectangleTopologyDir::NegativeX): return { index.value.x - 1, index.value.y };
		case static_cast<uint8_t>(RectangleTopologyDir::PositiveY): return { index.value.x, index.value.y + 1 };
		case static_cast<uint8_t>(RectangleTopologyDir::NegativeY): return { index.value.x, index.value.y - 1 };
		default:
			UC_ASSERT_ALWAYS_MSG("Invalid dir");
			return index;
		}
	}
}