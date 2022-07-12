#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Render.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Math.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vertex.hpp"

namespace unicore
{
	class Surface;
	class Texture;

	enum class Render2DFlag
	{
		FlipX = 1,
		FlipY = 1,
	};
	UNICORE_ENUMFLAGS(Render2DFlag, RenderFlags);

	enum class GeometryType
	{
		Points,
		LineList,
		LineStrip,
		Rects,
		RectsFilled,
	};

	class Render2D : public Render
	{
		UC_OBJECT(Render2D, Render)
	public:
		explicit Render2D(Logger& logger);

		virtual void clear(const Color4b& color) = 0;

		virtual void set_clip(Optional<Recti> clip_rect = std::nullopt) = 0;
		UC_NODISCARD virtual Optional<Recti> get_clip() const = 0;

		virtual void set_color(const Color4b& color) = 0;
		UC_NODISCARD virtual const Color4b& get_color() const = 0;

		virtual void draw_geometry(GeometryType type, const Vector2f* points, size_t points_count) = 0;

		virtual void draw_triangles(
			const VertexColor2* vertices, size_t num_vertices
		) = 0;

		virtual void draw_triangles(
			const VertexTexColor2* vertices, size_t num_vertices,
			const Texture* texture = nullptr
		) = 0;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

		static Unique<Render2D> create(Logger& logger);
	};
}