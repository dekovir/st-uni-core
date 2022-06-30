#include "unicore/Surface.hpp"

namespace unicore
{
	Surface::Surface()
		: _size(0, 0), _chunk(0)
	{
	}

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
		: _size(std::move(other._size)), _chunk(std::move(other._chunk))
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
		_size = std::move(other._size);
		_chunk = std::move(other._chunk);
		return *this;
	}

	void Surface::fill(const FillValue& value)
	{
		const auto ptr = reinterpret_cast<uint32_t*>(_chunk.data());
		const auto count = _size.area();

		if (const auto color = std::get_if<Color4b>(&value))
		{
			const auto data = color->to_format(pixel_format_abgr);
			for (int i = 0; i < count; i++)
				ptr[i] = data;
		}
		else if (const auto func = std::get_if<FillFunction>(&value))
		{
			int x = 0, y = 0;
			for (int i = 0; i < count; i++)
			{
				const auto data = (*func)(x, y).to_format(pixel_format_abgr);
				ptr[i] = data;

				x++;
				if (x == _size.x)
				{
					y++;
					x = 0;
				}
			}
		}
		else
		{
			UC_ASSERT_ALWAYS_MSG("Unimplemented");
		}
	}

	void Surface::fill(const Recti& rect, const FillValue& value)
	{
		// TODO: Optimize
		IBuffer2<Color4b>::fill(rect, value);
	}

	bool Surface::get(int x, int y, Color4b& value) const
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

	bool Surface::set(int x, int y, Color4b value)
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