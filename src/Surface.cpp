#include "unicore/Surface.hpp"

namespace unicore
{
	Surface::Surface(const Vector2i& size, MemoryChunk&& chunk)
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

	unsigned Surface::get_line_h(int x_, int y_, unsigned lng_, Color4b* dest) const
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.x - x);
		const auto data = static_cast<const UInt32*>(_chunk.data());

		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			dest[i] = Color4b::from_format(pixel_format_abgr, data[offset + i]);
		return lng;
	}

	unsigned Surface::get_line_v(int x_, int y_, unsigned lng_, Color4b* dest) const
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.y - y);
		const auto data = static_cast<const UInt32*>(_chunk.data());

		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			dest[i] = Color4b::from_format(pixel_format_abgr, data[offset + _size.x * i]);
		return lng;
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

	unsigned DynamicSurface::set_line_h(int x_, int y_, unsigned lng_, const Color4b& value)
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.x - x);
		const auto data = static_cast<UInt32*>(_chunk.data());

		const auto color = value.to_format(pixel_format_abgr);
		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			data[offset + i] = color;
		return lng;
	}

	unsigned DynamicSurface::set_line_v(int x_, int y_, unsigned lng_, const Color4b& value)
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.y - y);
		const auto data = static_cast<UInt32*>(_chunk.data());

		const auto color = value.to_format(pixel_format_abgr);
		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			data[offset + _size.x * i] = color;
		return lng;
	}

	unsigned DynamicSurface::set_line_h(int x_, int y_, unsigned lng_, const Color4b* src)
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.x - x);
		const auto data = static_cast<UInt32*>(_chunk.data());

		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			data[offset + i] = src[i].to_format(pixel_format_abgr);
		return lng;
	}

	unsigned DynamicSurface::set_line_v(int x_, int y_, unsigned lng_, const Color4b* src)
	{
		const auto x = Math::clamp(x_, 0, _size.x);
		const auto y = Math::clamp(y_, 0, _size.y);
		const auto lng = Math::min<unsigned>(lng_, _size.y - y);
		const auto data = static_cast<UInt32*>(_chunk.data());

		const auto offset = y * _size.x + x;
		for (unsigned i = 0; i < lng; i++)
			data[offset + _size.x * i] = src[i].to_format(pixel_format_abgr);
		return lng;
	}
}