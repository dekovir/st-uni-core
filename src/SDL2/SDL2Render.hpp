#pragma once
#include "unicore/Render.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Surface : public Surface
	{
	public:
		explicit SDL2Surface(SDL_Surface* context) : _context(context) { update_size(); }
		~SDL2Surface()
		{
			SDL_FreeSurface(_context);
		}

		const Vector2i& size() const override { return _size; }

	protected:
		SDL_Surface* _context;
		Vector2i _size;

		void update_size();

		friend class SDL2Render;
	};

	class SDL2Texture : public Texture
	{
	public:
		explicit SDL2Texture(SDL_Texture* context) : _context(context) { update_size(); }
		~SDL2Texture()
		{
			SDL_DestroyTexture(_context);
		}

		const Vector2i& size() const override { return _size; }

	protected:
		SDL_Texture* _context;
		Vector2i _size;

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

	class SDL2Render : public Render
	{
	public:
		explicit SDL2Render(const SDL2RenderSettings& settings);
		~SDL2Render() override;

		const Vector2i& screen_size() const override { return _size; }
		const RenderState& state() const override { return _state; }

		void set_state(const RenderState& state) override;

		Shared<Surface> load_surface(const Shared<ReadStream>& stream) override;
		Shared<Texture> create_texture(Surface& surface) override;

		bool begin_scene() override;
		void end_scene() override;

		void clear(const Color4b& color) override;

		void draw_geometry(const Vertex* vertices, size_t num_vertices) override;

	protected:
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		Vector2i _size;
		RenderState _state;

		void update_size();

		friend class SDL2Platform;
	};
}

#endif
