#include "unicore/Graphics.hpp"

namespace unicore
{
	Graphics::Graphics(RenderQueue& queue, GraphicsFlags flags)
		: _queue(queue), _color(Colors4b::White), _flags(flags)
	{
		if (_flags.HasFlag(GraphicsFlag::CallBegin))
			_queue.begin();
	}

	Graphics::~Graphics()
	{
		if (_flags.HasFlag(GraphicsFlag::CallEnd))
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
			.add_vertex({ p, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_point(const Vector2f& p)
	{
		_queue
			.set_primitive_type(PrimitiveType::Points)
			.add_vertex({ p, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		_queue
			.set_primitive_type(PrimitiveType::LineList)
			.add_vertex({ p0, _color })
			.add_vertex({ p1, _color })
			;
		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		_queue
			.set_primitive_type(PrimitiveType::LineList)
			.add_vertex({ p0, _color })
			.add_vertex({ p1, _color })
			;
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

	Graphics& Graphics::draw_sprite(const Shared<Texture>& texture, const Vector2f& center)
	{
		const auto& size = texture->size();
		const auto hx = static_cast<float>(size.x) / 2;
		const auto hy = static_cast<float>(size.y) / 2;

		const Vertex v0{ Vector2f(center.x + hx, center.y + hy), _color };
		const Vertex v1{ Vector2f(center.x + hx, center.y + hy), _color };
		const Vertex v2{ Vector2f(center.x + hx, center.y + hy), _color };
		const Vertex v3{ Vector2f(center.x + hx, center.y + hy), _color };

		return draw_quad(v0, v1, v2, v3, texture);
	}
}