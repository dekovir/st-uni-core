#include "SDL2Render.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"
#include "SDL2Loaders.hpp"

namespace unicore
{
	static List<SDL_Point> s_points;
	static List<SDL_FPoint> s_points_f;

	static List<SDL_Rect> s_rects;
	static List<SDL_FRect> s_rects_f;

	static std::vector<SDL_Vertex> s_vertices;

	SDL2Texture::SDL2Texture(SDL_Texture* context)
		: _context(context)
	{
		update_size();
	}

	uint32_t SDL2Texture::video_memory_use() const
	{
		Uint32 format;
		if (SDL_QueryTexture(_context, &format, nullptr, nullptr, nullptr) == 0)
		{
			const auto bpp = SDL_BYTESPERPIXEL(format);
			return _size.area() * bpp;
		}

		return 0;
	}

	void SDL2Texture::update_size()
	{
		SDL_QueryTexture(_context, nullptr, nullptr, &_size.x, &_size.y);
	}

	SDL2Render::SDL2Render(Logger& logger, const SDL2RenderSettings& settings)
		: Render2D(logger)
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		//SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

		Uint32 flags = SDL_WINDOW_OPENGL;
		if (settings.resizeable)
			flags |= SDL_WINDOW_RESIZABLE;
		if (settings.borderless)
			flags |= SDL_WINDOW_BORDERLESS;
		if (settings.fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN;

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

		auto width = settings.size.x;
		auto height = settings.size.y;
		if (width <= 0 || height <= 0)
		{
			SDL_DisplayMode dm;
			if (SDL_GetCurrentDisplayMode(0, &dm) == 0)
			{
				width = dm.w;
				height = dm.h;
			}
			else
			{
				width = 800;
				height = 600;
			}
		}

		_window = SDL_CreateWindow(settings.title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height, flags);

		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

		SDL_RendererInfo info;
		if (SDL_GetRendererInfo(_renderer, &info) == 0)
		{
			UC_LOG_INFO(_logger) << "Using " << (info.name ? info.name : "null") << " device";
		}
		else
		{
			UC_LOG_WARNING(_logger) << "Failed to get render info";
		}

		update_size();
	}

	SDL2Render::~SDL2Render()
	{
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
	}

	Shared<Texture> SDL2Render::create_texture(Surface& surface)
	{
		auto& size = surface.size();
		// TODO: Implement different formats
#if 1
		auto sdl_texture = SDL_CreateTexture(_renderer,
			SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, size.x, size.y);
		SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
		SDL_UpdateTexture(sdl_texture, nullptr, surface.data(), size.x * 4);
		return make_shared<SDL2Texture>(sdl_texture);
#else
		if (const auto sdl_surface = SDL_CreateRGBSurfaceWithFormatFrom(
			surface.data(), size.x, size.y, 4 * 8, size.x * 4, SDL_PIXELFORMAT_RGBA32))
		{
			auto texture = SDL_CreateTextureFromSurface(_renderer, sdl_surface);
			SDL_FreeSurface(sdl_surface);
			if (texture)
				return make_shared<SDL2Texture>(texture);
		}
		return nullptr;
#endif
	}

	bool SDL2Render::begin_scene()
	{
		set_color(ColorConst4b::White);
		set_clip(std::nullopt);
		return true;
	}

	void SDL2Render::end_scene()
	{
		SDL_RenderPresent(_renderer);
	}

	void SDL2Render::clear(const Color4b& color)
	{
		SDL_SetRenderDrawColor(_renderer,
			color.r, color.g, color.b, color.a);
		SDL_RenderClear(_renderer);
	}

	void SDL2Render::set_clip(Optional<Recti> clip_rect)
	{
		_clip_rect = clip_rect;
		if (_clip_rect.has_value())
		{
			SDL_Rect rect;
			SDL2Utils::convert(clip_rect.value(), rect);
			SDL_RenderSetClipRect(_renderer, &rect);
		}
		else
		{
			SDL_RenderSetClipRect(_renderer, nullptr);
		}
	}

	void SDL2Render::set_color(const Color4b& color)
	{
		_color = color;
		SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
	}

	void SDL2Render::draw_points(const Vector2i* points, size_t count)
	{
		SDL2Utils::convert(points, count, s_points);
		SDL_RenderDrawPoints(_renderer, s_points.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_points_f(const Vector2f* points, size_t count)
	{
		SDL2Utils::convert(points, count, s_points_f);
		SDL_RenderDrawPointsF(_renderer, s_points_f.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_lines(const Vector2i* points, size_t count)
	{
		SDL2Utils::convert(points, count, s_points);
		SDL_RenderDrawLines(_renderer, s_points.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_lines_f(const Vector2f* points, size_t count)
	{
		SDL2Utils::convert(points, count, s_points_f);
		SDL_RenderDrawLinesF(_renderer, s_points_f.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_rects(const Recti* rect, size_t count, bool filled)
	{
		SDL2Utils::convert(rect, count, s_rects);
		if (filled)
			SDL_RenderFillRects(_renderer, s_rects.data(), static_cast<int>(count));
		else
			SDL_RenderDrawRects(_renderer, s_rects.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_rects_f(const Rectf* rect, size_t count, bool filled)
	{
		SDL2Utils::convert(rect, count, s_rects_f);
		if (filled)
			SDL_RenderFillRectsF(_renderer, s_rects_f.data(), static_cast<int>(count));
		else
			SDL_RenderDrawRectsF(_renderer, s_rects_f.data(), static_cast<int>(count));
	}

	void SDL2Render::draw_triangles(
		const VertexColor2* vertices, size_t num_vertices)
	{
		s_vertices.resize(num_vertices);
		for (size_t i = 0; i < num_vertices; i++)
		{
			const auto& vertex = vertices[i];
			auto& [position, color, uv] = s_vertices[i];

			position.x = vertex.pos.x;
			position.y = vertex.pos.y;
			color.r = vertex.col.r;
			color.g = vertex.col.g;
			color.b = vertex.col.b;
			color.a = vertex.col.a;
		}

		SDL_RenderGeometry(_renderer, nullptr,
			s_vertices.data(), static_cast<int>(num_vertices),
			nullptr, 0
		);
	}

	void SDL2Render::draw_triangles(const VertexTexColor2* vertices,
		size_t num_vertices, const Texture* texture)
	{
		const auto tex = dynamic_cast<const SDL2Texture*>(texture);
		const auto tex_handle = tex ? tex->_context : nullptr;

		s_vertices.resize(num_vertices);
		for (size_t i = 0; i < num_vertices; i++)
		{
			const auto& vertex = vertices[i];
			auto& [position, color, uv] = s_vertices[i];

			position.x = vertex.pos.x;
			position.y = vertex.pos.y;
			uv.x = vertex.uv.x;
			uv.y = vertex.uv.y;
			color.r = vertex.col.r;
			color.g = vertex.col.g;
			color.b = vertex.col.b;
			color.a = vertex.col.a;
		}

		SDL_RenderGeometry(_renderer, tex_handle,
			s_vertices.data(), static_cast<int>(num_vertices),
			nullptr, 0
		);
	}

	void SDL2Render::update_size()
	{
		SDL_GetRendererOutputSize(_renderer, &_size.x, &_size.y);
	}

	SDL_RendererFlip SDL2Render::convert_flip(RenderFlags flags)
	{
		int value = SDL_FLIP_NONE;
		if (flags.has(Render2DFlag::FlipX))
			value |= SDL_FLIP_HORIZONTAL;
		if (flags.has(Render2DFlag::FlipY))
			value |= SDL_FLIP_VERTICAL;
		return static_cast<SDL_RendererFlip>(value);
	}
}

#endif