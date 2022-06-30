#pragma once
#include "unicore/Memory.hpp"
#include "unicore/Buffer2.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class Surface : public Resource
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;
	};

	class BitmapSurface : public Surface, public IBuffer2<Color4b>
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

		UC_NODISCARD bool get(int x, int y, Color4<unsigned char>& value) const override;
		bool set(int x, int y, Color4<unsigned char> value) override;

	protected:
		Vector2i _size = VectorConst2i::Zero;
		MemoryChunk _chunk;

		UC_NODISCARD int calc_offset(int x, int y) const { return (y * _size.x + x) * 4; }
	};
}