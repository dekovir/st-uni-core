#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename T>
	class IBuffer2;

	template<typename T>
	class IReadOnlyBuffer2
	{
	public:
		UC_NODISCARD virtual const Vector2i& size() const = 0;

		UC_NODISCARD virtual bool get(int x, int y, T& value) const = 0;
		UC_NODISCARD virtual bool get(const Vector2i& pos, T& value) const { return get(pos.x, pos.y, value); }

		virtual void copy(const Recti& src_rect, IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = dest.size();

			const Recti rect(
				offset.x, offset.y,
				Math::min(src_rect.w, s.x - offset.x),
				Math::min(src_rect.h, s.y - offset.y)
			);
			dest.fill([&](int x, int y) -> T
				{
					T value;
					return get(src_rect.x + x, src_rect.y + y, value) ? value : T{};
				}, rect);
		}

		virtual void copy(IBuffer2<T>& dest, const Vector2i& offset = VectorConst2i::Zero) const
		{
			const auto s = size();
			const Recti rect{ 0, 0, s.x, s.y };
			copy(rect, dest, offset);
		}
	};

	template<typename T>
	class IBuffer2 : public IReadOnlyBuffer2<T>
	{
	public:
		using FillFunction = std::function<T(int x, int y)>;

		virtual ~IBuffer2() = default;

		virtual bool set(int x, int y, T value) = 0;
		virtual bool set(const Vector2i& pos, T value) { return set(pos.x, pos.y, value); }

		virtual void clip_rect(Recti& rect) const
		{
			if (rect.x < 0)
			{
				rect.w += rect.x;
				rect.x = 0;
			}

			if (rect.y < 0)
			{
				rect.h += rect.y;
				rect.y = 0;
			}

			const auto s = this->size();
			if (rect.x + rect.w > s.x)
				rect.w = s.x - rect.x;

			if (rect.y + rect.h > s.y)
				rect.h = s.y - rect.y;
		}

		UC_NODISCARD virtual Recti clip_rect(const Recti& rect) const
		{
			Recti result(rect);
			clip_rect(result);
			return result;
		}

		virtual void fill(const T& value, const Optional<Recti>& rect = std::nullopt)
		{
			const auto s = this->size();
			if (rect.has_value())
			{
				const auto r = clip_rect(rect.value());
				for (auto y = r.min_y(); y < r.max_y(); ++y)
					for (auto x = r.min_x(); x < r.max_x(); ++x)
						set(x, y, value);
			}
			else
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
					{
						set(x, y, value);
					}
			}
		}

		virtual void fill(FillFunction func, const Optional<Recti>& rect = std::nullopt)
		{
			const auto s = this->size();
			if (rect.has_value())
			{
				const auto r = clip_rect(rect.value());
				for (auto y = r.min_y(); y < r.max_y(); ++y)
					for (auto x = r.min_x(); x < r.max_x(); ++x)
						set(x, y, func(x, y));
			}
			else
			{
				for (auto y = 0; y < s.y; y++)
					for (auto x = 0; x < s.x; x++)
						set(x, y, func(x, y));
			}
		}
	};

	template<typename T>
	class Buffer2 : public IBuffer2<T>
	{
	public:
		explicit Buffer2(const Vector2i& size)
			: _data(size.area()), _size(size)
		{}

		Buffer2(int x, int y)
			: _data(x* y), _size(x, y)
		{}

		T* data() { return _data.data(); }
		const T* data() const { return _data.data(); }

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD bool get(int x, int y, T& value) const override
		{
			if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
			{
				auto offset = calc_offset(x, y);
				value = _data[offset];
				return true;
			}

			return false;
		}

		bool set(int x, int y, T value) override
		{
			if (x >= 0 && x < _size.x && y >= 0 && y < _size.y)
			{
				auto offset = calc_offset(x, y);
				_data[offset] = value;
				return true;
			}

			return false;
		}

		UC_NODISCARD constexpr int calc_offset(int x, int y) const { return y * _size.x + x; }
		UC_NODISCARD constexpr int calc_offset(const Vector2i& index) const { return calc_offset(index.x, index.y); }

		void resize(const Vector2i& size, const T& value)
		{
			_data.resize(size.area(), value);
		}

	protected:
		List<T> _data;
		Vector2i _size;
	};

	template<typename T>
	class BufferPainter
	{
	public:
		explicit BufferPainter(IBuffer2<T>& buffer)
			: _buffer(buffer)
		{
		}

		void draw_line_h(int x, int y, unsigned length, const T& value)
		{
			for (unsigned i = 0; i < length; i++)
				_buffer.set(x + i, y, value);
		}

		void draw_line_h(const Vector2i& start, unsigned length, const T& value)
		{
			draw_line_h(start.x, start.y, length, value);
		}

		void draw_line_v(int x, int y, unsigned length, const T& value)
		{
			for (unsigned i = 0; i < length; i++)
				_buffer.set(x, y + i, value);
		}

		void draw_line_v(const Vector2i& start, unsigned length, const T& value)
		{
			draw_line_v(start.x, start.y, length, value);
		}

		void draw_rect(const Recti& rect, const T& value)
		{
			draw_line_h(rect.x, rect.y, rect.w, value);
			draw_line_h(rect.x, rect.y + rect.h, rect.w, value);

			draw_line_v(rect.x, rect.y + 1, rect.h - 2, value);
			draw_line_v(rect.x + rect.w, rect.y + 1, rect.h - 2, value);
		}

		void draw_line(int x1, int y1, int x2, int y2, const T& value)
		{
			int x, y;

			const int dx = x2 - x1;
			const int dy = y2 - y1;
			const int dx1 = Math::abs(dx);
			const int dy1 = Math::abs(dy);
			int px = 2 * dy1 - dx1;
			int py = 2 * dx1 - dy1;
			if (dy1 <= dx1)
			{
				int xe;
				if (dx >= 0)
				{
					x = x1;
					y = y1;
					xe = x2;
				}
				else
				{
					x = x2;
					y = y2;
					xe = x1;
				}
				_buffer.set(x, y, value);
				for (int i = 0; x < xe; i++)
				{
					x = x + 1;
					if (px < 0)
					{
						px = px + 2 * dy1;
					}
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						{
							y = y + 1;
						}
						else
						{
							y = y - 1;
						}
						px = px + 2 * (dy1 - dx1);
					}
					_buffer.set(x, y, value);
				}
			}
			else
			{
				int ye;
				if (dy >= 0)
				{
					x = x1;
					y = y1;
					ye = y2;
				}
				else
				{
					x = x2;
					y = y2;
					ye = y1;
				}
				_buffer.set(x, y, value);
				for (int i = 0; y < ye; i++)
				{
					y = y + 1;
					if (py <= 0)
					{
						py = py + 2 * dx1;
					}
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						{
							x = x + 1;
						}
						else
						{
							x = x - 1;
						}
						py = py + 2 * (dx1 - dy1);
					}
					_buffer.set(x, y, value);
				}
			}
		}

		void draw_line(const Vector2i& p0, const Vector2i& p1, const T& value)
		{
			draw_line(p0.x, p0.y, p1.x, p1.y, value);
		}

		void draw_circle(int center_x, int center_y, int radius, const T& value)
		{
			int x = 0, y = radius;
			int d = 3 - 2 * radius;
			draw_circle_points(center_x, center_y, x, y, value);
			while (y >= x)
			{
				x++;
				if (d > 0)
				{
					y--;
					d = d + 4 * (x - y) + 10;
				}
				else
					d = d + 4 * x + 6;
				draw_circle_points(center_x, center_y, x, y, value);
			}
		}

		void draw_circle(const Vector2i& center, int radius, const T& value)
		{
			draw_circle(center.x, center.y, radius, value);
		}

	protected:
		IBuffer2<T>& _buffer;

		void draw_circle_points(int xc, int yc, int x, int y, const T& value)
		{
			_buffer.set(xc + x, yc + y, value);
			_buffer.set(xc - x, yc + y, value);
			_buffer.set(xc + x, yc - y, value);
			_buffer.set(xc - x, yc - y, value);
			_buffer.set(xc + y, yc + x, value);
			_buffer.set(xc - y, yc + x, value);
			_buffer.set(xc + y, yc - x, value);
			_buffer.set(xc - y, yc - x, value);
		}
	};
}