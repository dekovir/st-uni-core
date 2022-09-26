#pragma once
#include "unicore/Rect.hpp"

namespace unicore
{
	template<typename TData, typename TValue, typename TAction>
	class Canvas
	{
	public:
		using ValueType = const TValue&;

		TData& data() { return _data; }
		const TData& data() const { return _data; }

		explicit Canvas(TData& data)
			: _data(data)
		{
		}

		void draw_point(int x, int y, ValueType value)
		{
			_action(_data, x, y, value);
		}

		void draw_point(const Vector2i& pos, ValueType value)
		{
			draw_point(pos.x, pos.y, value);
		}

		// TODO: Replace with span
		void draw_points(const Vector2i* points, unsigned num_points, ValueType value)
		{
			for (unsigned i = 0; i < num_points; i++)
				_action(_data, points[i].x, points[i].y, value);
		}

		void draw_line_h(int x, int y, unsigned length, ValueType value)
		{
			for (unsigned i = 0; i < length; i++)
				_action(_data, x + i, y, value);
		}

		void draw_line_h(const Vector2i& start, unsigned length, ValueType value)
		{
			draw_line_h(start.x, start.y, length, value);
		}

		void draw_line_v(int x, int y, unsigned length, ValueType value)
		{
			for (unsigned i = 0; i < length; i++)
				_action(_data, x, y + i, value);
		}

		void draw_line_v(const Vector2i& start, unsigned length, ValueType value)
		{
			draw_line_v(start.x, start.y, length, value);
		}

		void draw_rect(int x, int y, int w, int h, ValueType value)
		{
			draw_line_h(x, y, w, value);
			draw_line_h(x, y + h, w, value);

			draw_line_v(x, y + 1, h - 2, value);
			draw_line_v(x + w, y + 1, h - 2, value);
		}

		void draw_rect(const Recti& rect, ValueType value)
		{
			draw_rect(rect.x, rect.y, rect.w, rect.h, value);
		}

		void fill_rect(int x, int y, int w, int h, ValueType value)
		{
			for (unsigned i = 0; i < h; i++)
				draw_line_h(x, y + i, w, value);
		}

		void fill_rect(const Recti& rect, ValueType value)
		{
			fill_rect(rect.x, rect.y, rect.w, rect.h, value);
		}

		void draw_line(int x1, int y1, int x2, int y2, ValueType value)
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
				_action(_data, x, y, value);
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
					_action(_data, x, y, value);
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
				_action(_data, x, y, value);
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
					_action(_data, x, y, value);
				}
			}
		}

		void draw_line(const Vector2i& p0, const Vector2i& p1, ValueType value)
		{
			draw_line(p0.x, p0.y, p1.x, p1.y, value);
		}

		// TODO: Replace with span
		void draw_line_poly(const Vector2i* points, unsigned num_points, ValueType value)
		{
			for (unsigned i = 1; i < num_points; i++)
				draw_line(points[i - 1], points[i], value);
		}

		void draw_line_poly(const List<Vector2i>& points, ValueType value)
		{
			draw_line_poly(points.data(), points.size(), value);
		}

		void draw_circle(int center_x, int center_y, int radius, ValueType value)
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

		void draw_circle(const Vector2i& center, int radius, ValueType value)
		{
			draw_circle(center.x, center.y, radius, value);
		}

		void fill_circle(int center_x, int center_y, int radius, ValueType value)
		{
			// TODO: Optimize
			const int N = 2 * radius + 1;
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					const int x = i - radius;
					const int y = j - radius;

					if (x * x + y * y <= radius * radius + 1)
						_action(_data, center_x + x, center_y + y, value);
				}
			}
		}

		void fill_circle(const Vector2i& center, int radius, ValueType value)
		{
			fill_circle(center.x, center.y, radius, value);
		}

		// TODO: Implement triangle rasterization
		// https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling

	protected:
		TData& _data;
		TAction _action;

		void draw_circle_points(int xc, int yc, int x, int y, ValueType value)
		{
			_action(_data, xc + x, yc + y, value);
			_action(_data, xc - x, yc + y, value);
			_action(_data, xc + x, yc - y, value);
			_action(_data, xc - x, yc - y, value);
			_action(_data, xc + y, yc + x, value);
			_action(_data, xc - y, yc + x, value);
			_action(_data, xc + y, yc - x, value);
			_action(_data, xc - y, yc - x, value);
		}
	};
}