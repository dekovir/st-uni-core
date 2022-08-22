#include "unicore/Surface.hpp"

namespace unicore
{
	Surface::Surface(int width, int height)
		: _size(width, height), _chunk(_size.area() * 4)
	{
	}

	Surface::Surface(const Vector2i& size)
		: _size(size), _chunk(_size.area() * 4)
	{
	}

	Surface::Surface(const Surface& other)
		: _size(other.size()), _chunk(other._chunk)
	{
	}

	Surface::Surface(Surface&& other) noexcept
		: _size(std::exchange(other._size, VectorConst2i::Zero))
		, _chunk(std::move(other._chunk))
	{
	}

	Surface& Surface::operator=(const Surface& other)
	{
		_size = other._size;
		_chunk = other._chunk;
		return *this;
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		_size = std::exchange(other._size, VectorConst2i::Zero);
		_chunk = std::move(other._chunk);
		return *this;
	}

	void Surface::fill(const Color4b& color, const Optional<Recti>& rect)
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

	void Surface::fill(FillFunction func, const Optional<Recti>& rect)
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

	bool Surface::set(int x, int y, Color4b value)
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