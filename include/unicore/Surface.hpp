#pragma once
#include "unicore/Memory.hpp"
#include "unicore/Buffer2.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	// TODO: Rename to something else (Bitmap, PixelBuffer, ColorBuffer)
	class Surface : public Resource, public IBuffer2<Color4b>
	{
		UC_OBJECT(Surface, Resource)
	public:
		Surface(int width, int height);
		explicit Surface(const Vector2i& size);

		explicit Surface(const Surface& other);
		Surface(Surface&& other) noexcept;

		~Surface() override = default;

		Surface& operator=(const Surface& other);
		Surface& operator=(Surface&& other) noexcept;

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(Surface) + _chunk.size(); }
		UC_NODISCARD const Vector2i& size() const override { return _size; }

		UC_NODISCARD auto data() { return _chunk.data(); }
		UC_NODISCARD auto data() const { return _chunk.data(); }
		UC_NODISCARD size_t data_size() const { return _chunk.size(); }

		void fill(const Color4b& color, const Optional<Recti>& rect = std::nullopt) override;
		void fill(FillFunction func, const Optional<Recti>& rect = std::nullopt) override;

		void clear() { fill(ColorConst4b::Black); }

		UC_NODISCARD bool get(int x, int y, Color4b& value) const override;
		bool set(int x, int y, Color4b value) override;

	protected:
		Vector2i _size = VectorConst2i::Zero;
		MemoryChunk _chunk;

		UC_NODISCARD int calc_offset(int x, int y) const { return (y * _size.x + x) * 4; }
	};
}