#pragma once
#include "unicore/renderer/sdl2/Pipeline.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Texture.hpp"
#include <SDL.h>

namespace unicore
{
	class Display;
	class SDL2Display;
	class SDL2TargetTexture;

	class SDL2RendererSDL : public sdl2::Pipeline
	{
	public:
		SDL2RendererSDL(Logger& logger, SDL2Display& display);
		~SDL2RendererSDL() override;

		UC_NODISCARD const Vector2i& screen_size() const override { return _size; }
		UC_NODISCARD uint32_t draw_calls() const override { return _draw_calls; }

		Shared<Texture> create_texture(Surface& surface) override;

		Shared<DynamicTexture> create_dynamic_texture(const Vector2i& size) override;
		bool update_texture(DynamicTexture& texture, Surface& surface, Optional<Recti> rect) override;

		Shared<TargetTexture> create_target_texture(const Vector2i& size) override;
		bool set_target(const Shared<TargetTexture>& texture) override;
		UC_NODISCARD const Shared<TargetTexture>& get_target() const override;

		bool begin_frame() override;
		void end_frame() override;

		void clear(const Color4b& color) override;

		// STATES
		void set_viewport(const Optional<Recti>& rect) override;
		UC_NODISCARD const Optional<Recti>& get_viewport() const override { return _viewport; }

		void set_scale(const Vector2f& value) override;
		UC_NODISCARD const Vector2f& get_scale() const override { return _scale; }

		void set_logical_size(const Vector2i& size) override;
		UC_NODISCARD const Vector2i& get_logical_size() const override { return _logical_size; }

		void set_clip(const Optional<Recti>& clip_rect) override;
		UC_NODISCARD const Optional<Recti>& get_clip() const override { return _clip_rect; }

		void set_draw_color(const Color4b& color) override;
		UC_NODISCARD const Color4b& get_draw_color() const override { return _color; }

		// POINTS ////////////////////////////////////////////////////////////////////
		void draw_pointi(const Vector2i& p) override;
		void draw_pointf(const Vector2f& p) override;

		void draw_pointsi(const Vector2i* points, unsigned count) override;
		void draw_pointsf(const Vector2f* points, unsigned count) override;

		// LINES /////////////////////////////////////////////////////////////////////
		void draw_linei(const Vector2i& p1, const Vector2i& p2) override;
		void draw_linef(const Vector2f& p1, const Vector2f& p2) override;

		void draw_poly_linei(const Vector2i* points, unsigned count) override;
		void draw_poly_linef(const Vector2f* points, unsigned count) override;

		// RECTS /////////////////////////////////////////////////////////////////////
		void draw_recti(const Recti& rect, bool filled) override;
		void draw_rectf(const Rectf& rect, bool filled) override;

		void draw_rectsi(const Recti* rects, unsigned count, bool filled) override;
		void draw_rectsf(const Rectf* rects, unsigned count, bool filled) override;

		// TRIANGLES /////////////////////////////////////////////////////////////////
		void draw_trianglesf(
			const VertexColor2* vertices, unsigned num_vertices) override;

		void draw_trianglesf(
			const VertexTexColor2* vertices, unsigned num_vertices,
			const Texture* texture = nullptr) override;

		// COPY //////////////////////////////////////////////////////////////////////
		bool copyi(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect) override;

		bool copyf(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect) override;

		bool copy_exi(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
			Degrees angle, const Optional<Vector2i>& center, sdl2::RenderFlip flip) override;

		bool copy_exf(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
			Degrees angle, const Optional<Vector2f>& center, sdl2::RenderFlip flip) override;

		static Unique<SDL2RendererSDL> create(Logger& logger, Display& display);

	protected:
		Logger& _logger;
		SDL2Display& _display;
		SDL_Renderer* _renderer;
		Vector2i _size = VectorConst2i::Zero;

		// STATES
		Optional<Recti> _viewport;
		Optional<Recti> _clip_rect;
		Vector2f _scale;
		Vector2i _logical_size;
		uint32_t _draw_calls = 0;
		Color4b _color = ColorConst4b::White;
		Shared<TargetTexture> _target;

		void update_size();
		void update_scale();
		void update_viewport();
		void update_logical_size();

		UC_NODISCARD SDL_Texture* create_texture(const Vector2i& size, SDL_TextureAccess access) const;

		static SDL_RendererFlip convert_flip(sdl2::RenderFlip flags);
	};
}

#endif