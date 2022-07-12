#pragma once
#include "unicore/Index.hpp"
#include "unicore/Vector2.hpp"
#include "unicore/Object.hpp"

namespace unicore
{
	struct MapIndexTag {};

	struct CellIndex : Index<Vector2i, MapIndexTag>
	{
		constexpr CellIndex(int x, int y) : Index({ x, y }) {}
	};

	class Topology : public Object
	{
		UC_OBJECT(Topology, Object)
	public:
		UC_NODISCARD virtual const Vector2f& size() const = 0;

		UC_NODISCARD virtual Vector2f cell_to_pos(const CellIndex index) const = 0;
		UC_NODISCARD virtual CellIndex pos_to_cell(const Vector2f& pos) const = 0;

		virtual void get_cell_points(const CellIndex index, List<Vector2f>& points) const = 0;
		UC_NODISCARD virtual CellIndex get_cell_neighbor(const CellIndex index, uint8_t dir) const = 0;
	};

	enum class RectangleTopologyDir : uint8_t
	{
		PositiveY,
		NegativeX,
		NegativeY,
		PositiveX,
	};

	class RectangleTopology : public Topology
	{
		UC_OBJECT(RectangleTopology, Topology)
	public:
		explicit RectangleTopology(const Vector2f& size)
			: _size(size), _gap(VectorConst2f::Zero)
		{}

		explicit RectangleTopology(const Vector2f& size, const Vector2f& gap)
			: _size(size), _gap(gap)
		{}

		UC_NODISCARD const Vector2f& size() const override { return _size; }
		UC_NODISCARD const Vector2f& gap() const { return _gap; }

		UC_NODISCARD Vector2f cell_to_pos(const CellIndex index) const override;
		UC_NODISCARD CellIndex pos_to_cell(const Vector2f& pos) const override;

		void get_cell_points(const CellIndex index, List<Vector2f>& points) const override;
		UC_NODISCARD CellIndex get_cell_neighbor(const CellIndex index, uint8_t dir) const override;

	protected:
		Vector2f _size;
		Vector2f _gap;
	};

	// Hexagon
	// Isometric
}