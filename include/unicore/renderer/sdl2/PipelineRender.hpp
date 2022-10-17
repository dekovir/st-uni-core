#pragma once
#include "unicore/system/EnumFlag.hpp"
#include "unicore/math/Rect.hpp"
#include "unicore/math/Vector2.hpp"
#include "unicore/renderer/Vertex.hpp"
#include "unicore/renderer/Texture.hpp"

namespace unicore::sdl2
{
	enum class FlipBit
	{
		Horizontal = 1 << 0,
		Vertical = 1 << 1,
	};
	UNICORE_ENUM_FLAGS(FlipBit, RenderFlip);

	class PipelineRender
	{
	public:
		virtual ~PipelineRender() = default;

		virtual void set_draw_color(const Color4b& color) = 0;
		UC_NODISCARD virtual const Color4b& get_draw_color() const = 0;

		// POINTS ////////////////////////////////////////////////////////////////////
		virtual void draw_pointi(const Vector2i& p) = 0;
		virtual void draw_pointf(const Vector2f& p) = 0;

		// TODO: Replace with span
		virtual void draw_pointsi(const Vector2i* points, unsigned count) = 0;
		// TODO: Replace with span
		virtual void draw_pointsf(const Vector2f* points, unsigned count) = 0;

		// LINES /////////////////////////////////////////////////////////////////////
		virtual void draw_linei(const Vector2i& p1, const Vector2i& p2) = 0;
		virtual void draw_linef(const Vector2f& p1, const Vector2f& p2) = 0;

		// TODO: Replace with span
		virtual void draw_poly_linei(const Vector2i* points, unsigned count) = 0;
		// TODO: Replace with span
		virtual void draw_poly_linef(const Vector2f* points, unsigned count) = 0;

		// RECTS /////////////////////////////////////////////////////////////////////
		virtual void draw_recti(const Recti& rect, bool filled) = 0;
		virtual void draw_rectf(const Rectf& rect, bool filled) = 0;

		// TODO: Replace with span
		virtual void draw_rectsi(const Recti* rects, unsigned count, bool filled) = 0;
		// TODO: Replace with span
		virtual void draw_rectsf(const Rectf* rects, unsigned count, bool filled) = 0;

		// TRIANGLES /////////////////////////////////////////////////////////////////
		// TODO: Replace with span
		virtual void draw_trianglesf(
			const VertexColor2f* vertices, unsigned num_vertices) = 0;

		// TODO: Replace with span
		virtual void draw_trianglesf(
		const VertexColorTexture2f* vertices, unsigned num_vertices,
			const Texture* texture = nullptr) = 0;

		// COPY //////////////////////////////////////////////////////////////////////
		virtual bool copyi(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect) = 0;

		virtual bool copyf(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect) = 0;

		virtual bool copy_exi(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
			Degrees angle, const Optional<Vector2i>& center, RenderFlip flip) = 0;

		virtual bool copy_exf(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
			Degrees angle, const Optional<Vector2f>& center, RenderFlip flip) = 0;
	};
}