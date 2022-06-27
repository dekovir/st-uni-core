#pragma once
#include "unicore/Render2D.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Surface : public Surface
	{
	public:
		explicit SDL2Surface(SDL_Surface* context);

		~SDL2Surface() override
		{
			SDL_FreeSurface(_context);
		}

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(SDL2Surface) + _context->h * _context->pitch; }

		UC_NODISCARD const Vector2i& size() const override { return _size; }

	protected:
		SDL_Surface* _context;
		Vector2i _size = Vector2i::Zero;

		void update_size();

		friend class SDL2Render;
	};

	class SDL2Texture : public Texture
	{
	public:
		explicit SDL2Texture(SDL_Texture* context);

		~SDL2Texture() override
		{
			SDL_DestroyTexture(_context);
		}

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(SDL2Texture); }

		UC_NODISCARD const Vector2i& size() const override { return _size; }

	protected:
		SDL_Texture* _context;
		Vector2i _size = Vector2i::Zero;

		void update_size();

		friend class SDL2Render;
	};

	struct SDL2RenderSettings
	{
		const char* title = "Title";
		Vector2i size = { 800, 600 };
		bool resizeable = false;
		bool borderless = false;
		bool fullscreen = false;
	};

	class SDL2Render : public Render2D
	{
	public:
		explicit SDL2Render(Logger& logger, const SDL2RenderSettings& settings);
		~SDL2Render() override;

		UC_NODISCARD const Vector2i& screen_size() const override { return _size; }

		Shared<Texture> create_texture(Surface& surface) override;

		bool begin_scene() override;
		void end_scene() override;

		void clear(const Color4b& color) override;

		void set_clip(Optional<Recti> clip_rect) override;
		UC_NODISCARD Optional<Recti> get_clip() const override { return _clip_rect; }

		void set_color(const Color4b& color) override;
		UC_NODISCARD const Color4b& get_color() const override { return _color; }

		void draw_points(const Vector2i* points, size_t count) override;
		void draw_points_f(const Vector2f* points, size_t count) override;

		void draw_lines(const Vector2i* points, size_t count) override;
		void draw_lines_f(const Vector2f* points, size_t count) override;

		void draw_rects(const Recti* rect, size_t count, bool filled) override;
		void draw_rects_f(const Rectf* rect, size_t count, bool filled) override;

		void draw_texture(const Texture& texture,
			const Render2DDrawTextureOptionsI& options) override;

		void draw_texture_f(const Texture& texture,
			const Render2DDrawTextureOptionsF& options) override;

		void draw_texture_ex(const Texture& texture,
			const Render2DDrawTextureOptionsExI& options) override;

		void draw_texture_exf(const Texture& texture,
			const Render2DDrawTextureOptionsExF& options) override;

		void draw_triangles(const Vertex2* vertices,
			size_t num_vertices, const Texture* texture) override;

		void register_module(Context& context) override;

	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;

		Vector2i _size = Vector2i::Zero;
		Color4b _color = ColorConst4b::White;
		Optional<Recti> _clip_rect;

		void update_size();

		static SDL_RendererFlip convert_flip(RenderFlags flags);

		friend class SDL2Platform;
	};
}

#endif