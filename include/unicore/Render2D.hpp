#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Render.hpp"
#include "unicore/Color4.hpp"
#include "unicore/Angle.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vertex2.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	enum class Render2DFlag
	{
		FlipX = 1,
		FlipY = 1,
	};
	UNICORE_ENUMFLAGS(Render2DFlag, RenderFlags);

	template<typename T>
	struct Render2DDrawTextureOptions
	{
		Optional<Recti> src_rect = std::nullopt;
		Optional<Rect<T>> dst_rect = std::nullopt;
	};
	using Render2DDrawTextureOptionsI = Render2DDrawTextureOptions<int>;
	using Render2DDrawTextureOptionsF = Render2DDrawTextureOptions<float>;

	template<typename T>
	struct Render2DDrawTextureOptionsEx
	{
		Optional<Recti> src_rect = std::nullopt;
		Optional<Rect<T>> dst_rect = std::nullopt;
		const Degrees& angle = DegreesConst::Zero;
		Optional<Vector2<T>> center = std::nullopt;
		RenderFlags flags = RenderFlags::Zero;
	};
	using Render2DDrawTextureOptionsExI = Render2DDrawTextureOptionsEx<int>;
	using Render2DDrawTextureOptionsExF = Render2DDrawTextureOptionsEx<float>;

	class Render2D : public Render
	{
	public:
		explicit Render2D(Logger& logger);

		virtual Shared<Texture> create_texture(Surface& surface) = 0;

		virtual void clear(const Color4b& color) = 0;

		virtual void set_clip(Optional<Recti> clip_rect = std::nullopt) = 0;
		UC_NODISCARD virtual Optional<Recti> get_clip() const = 0;

		virtual void set_color(const Color4b& color) = 0;
		UC_NODISCARD virtual const Color4b& get_color() const = 0;

		virtual void draw_points(const Vector2i* points, size_t count) = 0;
		virtual void draw_points_f(const Vector2f* points, size_t count) = 0;

		virtual void draw_lines(const Vector2i* points, size_t count) = 0;
		virtual void draw_lines_f(const Vector2f* points, size_t count) = 0;

		virtual void draw_rects(const Recti* rect, size_t count, bool filled = false) = 0;
		virtual void draw_rects_f(const Rectf* rect, size_t count, bool filled = false) = 0;

		virtual void draw_texture(const Texture& texture,
			const Render2DDrawTextureOptionsI& options = {}) = 0;

		virtual void draw_texture_f(const Texture& texture,
			const Render2DDrawTextureOptionsF& options = {}) = 0;

		virtual void draw_texture_ex(const Texture& texture,
			const Render2DDrawTextureOptionsExI& options = {}) = 0;

		virtual void draw_texture_exf(const Texture& texture,
			const Render2DDrawTextureOptionsExF& options = {}) = 0;

		virtual void draw_triangles(
			const Vertex2* vertices, size_t num_vertices,
			const Texture* texture = nullptr
		) = 0;

		void register_module(Context& context) override;
		void unregister_module(Context& context) override;

		static Shared<Render2D> create(Logger& logger);
	};
}