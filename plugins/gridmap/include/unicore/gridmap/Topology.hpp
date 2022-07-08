#pragma once
#include "unicore/Index.hpp"
#include "unicore/Vector2.hpp"
#include "unicore/Object.hpp"

namespace unicore
{
	struct MapIndexTag {};

	struct MapIndex : Index<Vector2i, MapIndexTag>
	{
		constexpr MapIndex(int x, int y) : Index({ x, y }) {}
	};

	class Topology : public Object
	{
	public:
		UC_NODISCARD virtual Vector2f cell_to_pos(const MapIndex index) const = 0;
		UC_NODISCARD virtual MapIndex pos_to_cell(const Vector2f& pos) const = 0;
	};

	class RectangleTopology : public Topology
	{
	public:
		explicit RectangleTopology(const Vector2f& size)
			: _size(size), _gap(VectorConst2f::Zero)
		{}

		explicit RectangleTopology(const Vector2f& size, const Vector2f& gap)
			: _size(size), _gap(gap)
		{}

		UC_NODISCARD const Vector2f& size() const { return _size; }
		UC_NODISCARD const Vector2f& gap() const { return _gap; }

		UC_NODISCARD Vector2f cell_to_pos(const MapIndex index) const override;
		UC_NODISCARD MapIndex pos_to_cell(const Vector2f& pos) const override;

	protected:
		Vector2f _size;
		Vector2f _gap;
	};

	// Hexagon
	// Isometric
}