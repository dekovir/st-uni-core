#pragma once
#include "unicore/Memory.hpp"
#include "unicore/Vector2.hpp"
#include "unicore/Resource.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
	class Surface : public Resource
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};

	class BitmapSurface : public Surface
	{
	public:
		BitmapSurface();
		BitmapSurface(int width, int height);
		explicit BitmapSurface(const Vector2i& size);

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(BitmapSurface) + _chunk.size(); }
		UC_NODISCARD const Vector2i& size() const override { return _size; }

		auto data() { return _chunk.data(); }
		UC_NODISCARD auto data() const { return _chunk.data(); }
		UC_NODISCARD size_t data_size() const { return _chunk.size(); }

		void fill(const Color4b& color);
		void clear() { fill(ColorConst4b::Black); }

		void set_pixel(int x, int y, const Color4b& color);
		void set_pixel(const Vector2i& pos, const Color4b& color) { set_pixel(pos.x, pos.y, color); }

		bool try_get_pixel(int x, int y, Color4b& color);
		bool try_get_pixel(const Vector2i& pos, Color4b& color) { return try_get_pixel(pos.x, pos.y, color); }

	protected:
		Vector2i _size = Vector2iConst::Zero;
		MemoryChunk _chunk;

		UC_NODISCARD int get_offset(int x, int y) const { return (y * _size.x + x) * 4; }
	};
}