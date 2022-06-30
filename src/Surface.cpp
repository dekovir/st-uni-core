#include "unicore/Surface.hpp"

namespace unicore
{
	BitmapSurface::BitmapSurface()
		: _size(0, 0), _chunk(0)
	{
	}

	BitmapSurface::BitmapSurface(int width, int height)
		: _size(width, height), _chunk(_size.area() * 4)
	{
	}

	BitmapSurface::BitmapSurface(const Vector2i& size)
		: _size(size), _chunk(_size.area() * 4)
	{
	}

	void BitmapSurface::fill(const Color4b& color)
	{
		const auto ptr = reinterpret_cast<uint32_t*>(_chunk.data());
		const auto count = _size.area();
		const auto data = color.to_format(pixel_format_abgr);

		for (int i = 0; i < count; i++)
			ptr[i] = data;
	}

	bool BitmapSurface::get(int x, int y, Color4b& value) const
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = calc_offset(x, y);
			const auto ptr = reinterpret_cast<const uint32_t*>(&_chunk.data()[offset]);
			value = Color4b::from_format(pixel_format_abgr, ptr[0]);
			return true;
		}

		return false;
	}

	bool BitmapSurface::set(int x, int y, Color4b value)
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = calc_offset(x, y);
			const auto ptr = reinterpret_cast<uint32_t*>(&_chunk.data()[offset]);
			ptr[0] = value.to_format(pixel_format_abgr);
			return true;
		}

		return false;
	}
}