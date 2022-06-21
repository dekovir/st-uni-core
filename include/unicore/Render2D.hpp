#pragma once
#include "unicore/Color4.hpp"
#include "unicore/Angle.hpp"
#include "unicore/Rect.hpp"
#include "unicore/FileSystem.hpp"
#include "unicore/Vertex2.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	enum class RenderFlag
	{
		FlipX = 1,
		FlipY = 1,
	};
	UNICORE_ENUMFLAGS(RenderFlag, RenderFlags);

	template<typename T>
	struct RenderDrawTextureOptions
	{
		Optional<Recti> src_rect = std::nullopt;
		Optional<Rect<T>> dst_rect = std::nullopt;
	};
	using RenderDrawTextureOptionsI = RenderDrawTextureOptions<int>;
	using RenderDrawTextureOptionsF = RenderDrawTextureOptions<float>;

	template<typename T>
	struct RenderDrawTextureOptionsEx
	{
		Optional<Recti> src_rect = std::nullopt;
		Optional<Rect<T>> dst_rect = std::nullopt;
		const Degrees& angle = DegreesConst::Zero;
		Optional<Vector2<T>> center = std::nullopt;
		RenderFlags flags = RenderFlags::Zero;
	};
	using RenderDrawTextureOptionsExI = RenderDrawTextureOptionsEx<int>;
	using RenderDrawTextureOptionsExF = RenderDrawTextureOptionsEx<float>;

	class Render2D : public Module
	{
	public:
		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;

		virtual Shared<Surface> load_surface(const Shared<ReadStream>& stream) = 0;
		virtual Shared<Texture> create_texture(Surface& surface) = 0;

		virtual Shared<Texture> load_texture(const Shared<ReadStream>& stream);

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;

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
			const RenderDrawTextureOptionsI& options = {}) = 0;

		virtual void draw_texture_f(const Texture& texture,
			const RenderDrawTextureOptionsF& options = {}) = 0;

		virtual void draw_texture_ex(const Texture& texture,
			const RenderDrawTextureOptionsExI& options = {}) = 0;

		virtual void draw_texture_exf(const Texture& texture,
			const RenderDrawTextureOptionsExF& options = {}) = 0;

		virtual void draw_triangles(
			const Vertex2* vertices, size_t num_vertices,
			const Texture* texture = nullptr
		) = 0;
	};
}