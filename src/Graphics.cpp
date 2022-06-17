#include "unicore/Graphics.hpp"

namespace unicore
{
	Graphics::Graphics(RenderQueue& queue, GraphicsFlags flags)
		: _queue(queue), _color(ColorConst4b::White), _flags(flags)
	{
		if (_flags.has(GraphicsFlag::CallBegin))
			_queue.begin();
	}

	Graphics::~Graphics()
	{
		if (_flags.has(GraphicsFlag::CallEnd))
			_queue.end();
	}

	Graphics& Graphics::set_color(const Color4b& color)
	{
		_color = color;
		return *this;
	}

	Graphics& Graphics::draw_point(const Vector2i& p)
	{
		_queue
			.set_primitive_type(PrimitiveType::Points)
			.add_vertex({ p.cast<float>(), Vector2f::Zero, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_point(const Vector2f& p)
	{
		_queue
			.set_primitive_type(PrimitiveType::Points)
			.add_vertex({ p, Vector2f::Zero, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		_queue
			.set_primitive_type(PrimitiveType::LineList)
			.add_vertex({ p0.cast<float>(), Vector2f::Zero, _color })
			.add_vertex({ p1.cast<float>(), Vector2f::Zero, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		_queue
			.set_primitive_type(PrimitiveType::LineList)
			.add_vertex({ p0, Vector2f::Zero, _color })
			.add_vertex({ p1, Vector2f::Zero, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_poly_line(std::initializer_list<Vector2f> points)
	{
		if (points.size() > 1)
		{
			_queue.set_primitive_type(PrimitiveType::LineList);
			auto data = points.begin();
			for (size_t i = 0; i + 1 < points.size(); i++)
			{
				_queue
					.add_vertex({ data[i + 0], Vector2f::Zero, _color })
					.add_vertex({ data[i + 1], Vector2f::Zero, _color });
			}
		}

		return *this;
	}

	Graphics& Graphics::draw_tri(
		const Vertex& v0, const Vertex& v1, const Vertex& v2,
		const Shared<Texture>& texture)
	{
		_queue
			.set_primitive_type(PrimitiveType::TriangleList)
			.set_texture(texture)
			.add_vertex(v0).add_vertex(v1).add_vertex(v2)
			;
		return *this;
	}

	Graphics& Graphics::draw_quad(
		const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3,
		const Shared<Texture>& texture)
	{
		_queue
			.set_primitive_type(PrimitiveType::TriangleList)
			.set_texture(texture)
			.add_vertex(v0).add_vertex(v1).add_vertex(v3)
			.add_vertex(v3).add_vertex(v1).add_vertex(v2)
			;
		return *this;
	}

	static Vertex s_quad[4];

	Graphics& Graphics::draw_sprite_at(const Shared<Texture>& texture, const Vector2f& center)
	{
		calc_quad_position(center, texture->size(),
			s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

		s_quad[0].uv = Vector2f(0, 0);
		s_quad[1].uv = Vector2f(1, 0);
		s_quad[2].uv = Vector2f(1, 1);
		s_quad[3].uv = Vector2f(0, 1);

		s_quad[0].col = _color;
		s_quad[1].col = _color;
		s_quad[2].col = _color;
		s_quad[3].col = _color;

		return draw_quad(s_quad[0], s_quad[1], s_quad[2], s_quad[3], texture);
	}

	Graphics& Graphics::draw_sprite_at(const Shared<Texture>& texture,
		const Vector2f& center, const Radians& angle, const Vector2f& scale)
	{
		calc_quad_position(center, texture->size(), angle, scale,
			s_quad[0].pos, s_quad[1].pos, s_quad[2].pos, s_quad[3].pos);

		s_quad[0].uv = Vector2f(0, 0);
		s_quad[1].uv = Vector2f(1, 0);
		s_quad[2].uv = Vector2f(1, 1);
		s_quad[3].uv = Vector2f(0, 1);

		s_quad[0].col = _color;
		s_quad[1].col = _color;
		s_quad[2].col = _color;
		s_quad[3].col = _color;

		return draw_quad(s_quad[0], s_quad[1], s_quad[2], s_quad[3], texture);
	}

	void Graphics::calc_quad_position(
		const Vector2f& center, const Vector2i& size,
		Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto hx = static_cast<float>(size.x) / 2;
		const auto hy = static_cast<float>(size.y) / 2;

		p0 = Vector2f(center.x - hx, center.y - hy);
		p1 = Vector2f(center.x + hx, center.y - hy);
		p2 = Vector2f(center.x + hx, center.y + hy);
		p3 = Vector2f(center.x - hx, center.y + hy);
	}

	static Vector2f rotate_and_scale(float x, float y,
		float a_cos, float a_sin, float scale_x, float scale_y)
	{
		return {
			a_cos * scale_x * x + a_sin * scale_y * y,
			a_cos * scale_y * y - a_sin * scale_x * x
		};
	}

	void Graphics::calc_quad_position(
		const Vector2f& center, const Vector2i& size, const Radians& angle,
		const Vector2f& scale, Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3)
	{
		const auto hx = static_cast<float>(size.x) / 2;
		const auto hy = static_cast<float>(size.y) / 2;

		const auto a_cos = angle.cos();
		const auto a_sin = angle.sin();

		p0 = center + rotate_and_scale(-hx, -hy, a_cos, a_sin, scale.x, scale.y);
		p1 = center + rotate_and_scale(+hx, -hy, a_cos, a_sin, scale.x, scale.y);
		p2 = center + rotate_and_scale(+hx, +hy, a_cos, a_sin, scale.x, scale.y);
		p3 = center + rotate_and_scale(-hx, +hy, a_cos, a_sin, scale.x, scale.y);
	}
}