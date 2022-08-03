#pragma once
#include "unicore/Renderer.hpp"
#include "unicore/EnumFlag.hpp"
#include "unicore/Vertex.hpp"

namespace unicore
{
	class Display;

	enum class SDLRenderFlip
	{
		Horizontal = 1 << 0,
		Vertical = 1 << 1,
	};
	UNICORE_ENUMFLAGS(SDLRenderFlip, SDLRenderFlipFlags);

	class RendererSDL : public Renderer
	{
		UC_OBJECT(RendererSDL, Renderer)
	public:
		explicit RendererSDL(Logger& logger);

		virtual void clear(const Color4b& color) = 0;

		// STATES
		virtual void set_viewport(const Optional<Recti>& rect) = 0;
		UC_NODISCARD virtual const Optional<Recti>& get_viewport() const = 0;

		virtual void set_scale(const Vector2f& value) = 0;
		UC_NODISCARD virtual const Vector2f& get_scale() const = 0;

		virtual void set_logical_size(const Vector2i& size) = 0;
		UC_NODISCARD virtual const Vector2i& get_logical_size() const = 0;

		virtual void set_clip(const Optional<Recti>& rect) = 0;
		UC_NODISCARD virtual const Optional<Recti>& get_clip() const = 0;

		virtual void set_draw_color(const Color4b& color) = 0;
		UC_NODISCARD virtual const Color4b& get_draw_color() const = 0;

		// DRAW POINTS
		virtual void draw_point(const Vector2i& p) = 0;
		virtual void draw_point_f(const Vector2f& p) = 0;

		virtual void draw_points(const Vector2i* points, unsigned count) = 0;
		virtual void draw_points_f(const Vector2f* points, unsigned count) = 0;

		// DRAW LINES
		virtual void draw_line(const Vector2i& p1, const Vector2i& p2) = 0;
		virtual void draw_line_f(const Vector2f& p1, const Vector2f& p2) = 0;

		virtual void draw_poly_line(const Vector2i* points, unsigned count) = 0;
		virtual void draw_poly_line_f(const Vector2f* points, unsigned count) = 0;

		// DRAW RECTS
		virtual void draw_rect(const Recti& rect, bool filled) = 0;
		virtual void draw_rect_f(const Rectf& rect, bool filled) = 0;

		virtual void draw_rects(const Recti* rects, unsigned count, bool filled) = 0;
		virtual void draw_rects_f(const Rectf* rects, unsigned count, bool filled) = 0;

		// DRAW TRIANGLES
		virtual void draw_triangles(
			const VertexColor2* vertices, unsigned num_vertices
		) = 0;

		virtual void draw_triangles(
			const VertexTexColor2* vertices, unsigned num_vertices,
			const Texture* texture = nullptr
		) = 0;

		// COPY TEXTURE
		virtual bool copy(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect) = 0;

		virtual bool copy_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect) = 0;

		virtual bool copy_ex(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
			Degrees angle, const Optional<Vector2i>& center, SDLRenderFlipFlags flip) = 0;

		virtual bool copy_ex_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
			Degrees angle, const Optional<Vector2f>& center, SDLRenderFlipFlags flip) = 0;

		static Unique<RendererSDL> create(Logger& logger, Display& display);
	};
}