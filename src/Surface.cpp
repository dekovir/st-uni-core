#include "unicore/Surface.hpp"

namespace unicore
{
	Surface::Surface(const Vector2i& size, MemoryChunk chunk)
		: _size(size), _chunk(std::move(chunk))
	{
	}

	bool Surface::get(int x, int y, Color4b& value) const
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = calc_offset(x, y);
			const auto data = static_cast<const uint8_t*>(_chunk.data());
			const auto ptr = reinterpret_cast<const uint32_t*>(&data[offset]);
			value = Color4b::from_format(pixel_format_abgr, ptr[0]);
			return true;
		}

		return false;
	}

	// DynamicSurface /////////////////////////////////////////////////////////////
	DynamicSurface::DynamicSurface(int width, int height)
		: Surface({ width, height }, MemoryChunk(width* height * 4))
	{
	}

	DynamicSurface::DynamicSurface(const Vector2i& size)
		: Surface(size, MemoryChunk(size.area() * 4))
	{
	}

	bool DynamicSurface::set(int x, int y, Color4b value)
	{
		if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
		{
			const auto offset = calc_offset(x, y);
			const auto data = static_cast<uint8_t*>(_chunk.data());
			const auto ptr = reinterpret_cast<uint32_t*>(&data[offset]);
			ptr[0] = value.to_format(pixel_format_abgr);
			return true;
		}

		return false;
	}
}