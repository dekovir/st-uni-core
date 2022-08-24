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

	void DynamicSurface::fill(const Color4b& color, const Optional<Recti>& rect)
	{
		const auto ptr = reinterpret_cast<uint32_t*>(_chunk.data());

		const auto data = color.to_format(pixel_format_abgr);
		if (rect.has_value())
		{
			const auto r = clip_rect(rect.value());
			for (auto y = r.min_y(); y < r.max_y(); y++)
			{
				const int offset = y * _size.x;
				for (auto x = r.min_x(); x < r.max_x(); x++)
					ptr[offset + x] = data;
			}
		}
		else
		{
			const auto count = _size.area();
			for (int i = 0; i < count; i++)
				ptr[i] = data;
		}
	}

	void DynamicSurface::fill(FillFunction func, const Optional<Recti>& rect)
	{
		const auto ptr = reinterpret_cast<uint32_t*>(_chunk.data());

		if (rect.has_value())
		{
			const auto r = clip_rect(rect.value());
			int offset = r.min_x() + r.min_y() * _size.x;
			for (auto y = r.min_y(); y < r.max_y(); y++)
			{
				for (auto x = r.min_x(); x < r.max_x(); x++)
				{
					const auto data = func(x, y).to_format(pixel_format_abgr);
					ptr[offset] = data;
					offset++;
				}
			}
		}
		else
		{
			int x = 0, y = 0;
			const auto count = _size.area();
			for (int i = 0; i < count; i++)
			{
				const auto data = func(x, y).to_format(pixel_format_abgr);
				ptr[i] = data;

				x++;
				if (x == _size.x)
				{
					y++;
					x = 0;
				}
			}
		}
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