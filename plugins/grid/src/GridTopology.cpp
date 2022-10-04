#include "unicore/grid/GridTopology.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	Vector2f RectangleTopology::cell_to_pos(const GridIndex index) const
	{
		// TODO: Implement gap
		return {
			(_size.x + _gap.x) * static_cast<float>(index.value.x),
			(_size.y + _gap.y) * static_cast<float>(index.value.y)
		};
	}

	GridIndex RectangleTopology::pos_to_cell(const Vector2f& pos) const
	{
		// TODO: Implement gap
		return GridIndex(
			Math::ceil(pos.x / _size.x),
			Math::ceil(pos.y / _size.y)
		);
	}

	void RectangleTopology::get_cell_points(const GridIndex& index, List<Vector2f>& points) const
	{
		const auto w = _size.x / 2;
		const auto h = _size.y / 2;
		const auto pos = cell_to_pos(index);

		points.emplace_back(pos.x - w, pos.y - h);
		points.emplace_back(pos.x + w, pos.y - h);
		points.emplace_back(pos.x + w, pos.y + h);
		points.emplace_back(pos.x - w, pos.y + h);
	}

	bool RectangleTopology::get_cell_neighbor(const GridIndex& index, uint8_t dir, GridIndex& neighbor) const
	{
		static constexpr GridIndex s_rect_dir_offset[4] = {
			GridIndex(0, +1), // PositiveY
			GridIndex(-1, 0), // NegativeX
			GridIndex(0, -1), // NegativeY
			GridIndex(+1, 0), // PositiveX
		};

		switch (dir)
		{
		case static_cast<uint8_t>(RectangleTopologyDir::PositiveX):
		case static_cast<uint8_t>(RectangleTopologyDir::NegativeX):
		case static_cast<uint8_t>(RectangleTopologyDir::PositiveY):
		case static_cast<uint8_t>(RectangleTopologyDir::NegativeY):
			neighbor = index + s_rect_dir_offset[dir];
			return true;

		default:
			return false;
		}
	}
}