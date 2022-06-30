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

	void BitmapSurface::fill(std::function<Color4b(int x, int y)> fill_func)
	{
		const auto ptr = reinterpret_cast<uint32_t*>(_chunk.data());
		const auto count = _size.area();

		int y = 0, x = 0;
		for (int i = 0; i < count; i++)
		{
			ptr[i] = fill_func(x, y).to_format(pixel_format_abgr);
			x++;
			if (x == _size.x)
			{
				y++;
				x = 0;
			}
		}
	}

	void BitmapSurface::set_pixel(int x, int y, const Color4b& color)
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = get_offset(x, y);
			const auto ptr = reinterpret_cast<uint32_t*>(&_chunk.data()[offset]);
			ptr[0] = color.to_format(pixel_format_abgr);
		}
	}

	bool BitmapSurface::try_get_pixel(int x, int y, Color4b& color)
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = get_offset(x, y);
			const auto ptr = reinterpret_cast<uint32_t*>(&_chunk.data()[offset]);
			color = Color4b::from_format(pixel_format_abgr, ptr[0]);
			return true;
		}

		return false;
	}
}