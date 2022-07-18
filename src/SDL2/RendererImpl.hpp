#pragma once
#include "unicore/SDLRenderer.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"
#include <SDL.h>

namespace unicore
{
	struct SDL2RenderSettings
	{
		const char* title = "Title";
		Vector2i size = { 800, 600 };
		bool resizeable = false;
		bool borderless = false;
		bool fullscreen = false;
	};

	class RendererImpl : public SDLRenderer
	{
		UC_OBJECT(RendererImpl, SDLRenderer)
	public:
		RendererImpl(Logger& logger, const SDL2RenderSettings& settings);
		~RendererImpl() override;

		UC_NODISCARD const Vector2i& screen_size() const override { return _size; }
		UC_NODISCARD uint32_t draw_calls() const override { return _draw_calls; }

		Shared<Texture> create_texture(Surface& surface) override;
		bool update_texture(Texture& texture, Surface& surface, Optional<Recti> rect) override;

		bool begin_scene() override;
		void end_scene() override;

		void clear(const Color4b& color) override;

		// STATES
		void set_viewport(const Optional<Recti>& rect) override;
		UC_NODISCARD const Optional<Recti>& get_viewport() const override { return _viewport; }

		void set_clip(const Optional<Recti>& clip_rect) override;
		UC_NODISCARD const Optional<Recti>& get_clip() const override { return _clip_rect; }

		void set_draw_color(const Color4b& color) override;
		UC_NODISCARD const Color4b& get_draw_color() const override { return _color; }

		// DRAW POINTS
		void draw_point(const Vector2i& p) override;
		void draw_point_f(const Vector2f& p) override;

		void draw_points(const Vector2i* points, unsigned count) override;
		void draw_points_f(const Vector2f* points, unsigned count) override;

		// DRAW LINES
		void draw_line(const Vector2i& p1, const Vector2i& p2) override;
		void draw_line_f(const Vector2f& p1, const Vector2f& p2) override;

		void draw_poly_line(const Vector2i* points, unsigned count) override;
		void draw_poly_line_f(const Vector2f* points, unsigned count) override;

		// DRAW RECTS
		void draw_rect(const Recti& rect, bool filled) override;
		void draw_rect_f(const Rectf& rect, bool filled) override;

		void draw_rects(const Recti* rects, unsigned count, bool filled) override;
		void draw_rects_f(const Rectf* rects, unsigned count, bool filled) override;

		// DRAW TRIANGLES
		void draw_triangles(
			const VertexColor2* vertices, unsigned num_vertices
		) override;

		void draw_triangles(
			const VertexTexColor2* vertices, unsigned num_vertices,
			const Texture* texture = nullptr
		) override;

		// COPY TEXTURE
		bool copy(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect) override;

		bool copy_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect) override;

		bool copy_ex(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
			Degrees angle, const Optional<Vector2i>& center, SDLRenderFlipFlags flip) override;

		bool copy_ex_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
			Degrees angle, const Optional<Vector2f>& center, SDLRenderFlipFlags flip) override;

	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		Vector2i _size = VectorConst2i::Zero;

		// STATES
		Optional<Recti> _viewport;
		Optional<Recti> _clip_rect;
		uint32_t _draw_calls = 0;
		Color4b _color = ColorConst4b::White;

		void update_size();

		static SDL_RendererFlip convert_flip(SDLRenderFlipFlags flags);
	};
}

#endif