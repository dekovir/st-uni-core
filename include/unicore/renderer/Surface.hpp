#pragma once
#include "unicore/system/Memory.hpp"
#include "unicore/system/Buffer2.hpp"
#include "unicore/resource/Resource.hpp"
#include "unicore/renderer/Color4.hpp"

namespace unicore
{
	class SolidSizeOptions : public ResourceOptions
	{
	public:
		Color4b color = ColorConst4b::White;
		Vector2i size = Vector2i(8);

		SolidSizeOptions() = default;

		explicit SolidSizeOptions(const Color4b& color_) : color(color_) {}
		SolidSizeOptions(const Color4b& color_, const Vector2i& size_) : color(color_), size(size_) {}

		UC_NODISCARD size_t hash() const override { return Hash::make(color, size); }
	};

	using SurfaceFormat = PixelFormat<uint32_t, uint8_t>;

	class BaseSurface : public Resource
	{
		UC_OBJECT(BaseSurface, Resource)
	public:
		UC_NODISCARD virtual SurfaceFormat format() const { return pixel_format_abgr; }
	};

	// TODO: Is Resource?
	// TODO: Rename to something else (Bitmap, PixelBuffer, ColorBuffer)
	class Surface : public BaseSurface, public IReadOnlyBuffer2<Color4b>
	{
		UC_OBJECT(Surface, Resource)
	public:
		Surface(const Vector2i& size, MemoryChunk&& chunk);

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(Surface) + _chunk.size(); }
		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD size_t size_bytes() const { return _chunk.size(); }

		UC_NODISCARD auto data() { return _chunk.data(); }
		UC_NODISCARD auto data() const { return _chunk.data(); }

		UC_NODISCARD bool get(int x, int y, Color4b& value) const override;

		unsigned get_line_h(int x, int y, unsigned lng, Color4b* dest) const override;
		unsigned get_line_v(int x, int y, unsigned lng, Color4b* dest) const override;

	protected:
		Vector2i _size = VectorConst2i::Zero;
		MemoryChunk _chunk;

		UC_NODISCARD int calc_offset(int x, int y) const { return (y * _size.x + x) * 4; }
	};

	class DynamicSurface : public Surface, public IBuffer2<Color4b>
	{
		UC_OBJECT(DynamicSurface, Resource)
	public:
		DynamicSurface(int width, int height);
		explicit DynamicSurface(const Vector2i& size);
		~DynamicSurface() override = default;

		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }
		UC_NODISCARD const Vector2i& size() const override { return Surface::size(); }
		UC_NODISCARD bool get(int x, int y, Color4<unsigned char>& value) const override { return Surface::get(x, y, value); }

		bool set(int x, int y, Color4b value) override;

		unsigned set_line_h(int x, int y, unsigned lng, const Color4b& value) override;
		unsigned set_line_v(int x, int y, unsigned lng, const Color4b& value) override;

		unsigned set_line_h(int x, int y, unsigned lng, const Color4b* src) override;
		unsigned set_line_v(int x, int y, unsigned lng, const Color4b* src) override;
	};
}