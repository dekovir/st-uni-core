#include "SDL2RendererSDL.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Texture.hpp"
#include "SDL2Loaders.hpp"
#include "SDL2Display.hpp"

namespace unicore
{
	static List<SDL_Point> s_points;
	static List<SDL_FPoint> s_points_f;

	static List<SDL_Rect> s_rects;
	static List<SDL_FRect> s_rects_f;

	static std::vector<SDL_Vertex> s_vertices;

	SDL2RendererSDL::SDL2RendererSDL(Logger& logger, SDL2Display& display)
		: ParentType(logger)
		, _display(display)
		, _scale(VectorConst2f::One)
		, _logical_size(VectorConst2i::Zero)
	{
		SDL_Init(SDL_INIT_VIDEO);

		_renderer = SDL_CreateRenderer(_display.handle(), -1, SDL_RENDERER_ACCELERATED);

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
		update_scale();
		update_viewport();
		update_logical_size();
	}

	SDL2RendererSDL::~SDL2RendererSDL()
	{
		SDL_DestroyRenderer(_renderer);
	}

	Shared<Texture> SDL2RendererSDL::create_texture(Surface& surface)
	{
		auto& size = surface.size();
		// TODO: Implement different formats
#if 1

		auto texture = create_texture(size, SDL_TEXTUREACCESS_STATIC);
		if (SDL_UpdateTexture(texture, nullptr, surface.data(), size.x * 4) == 0)
			return make_shared<SDL2Texture>(texture);

		UC_LOG_WARNING(_logger) << SDL_GetError();
		SDL_DestroyTexture(texture);
		return nullptr;

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

	Shared<DynamicTexture> SDL2RendererSDL::create_dynamic_texture(const Vector2i& size)
	{
		auto tex = create_texture(size, SDL_TEXTUREACCESS_STREAMING);
		if (!tex)
		{
			UC_LOG_ERROR(_logger) << "Failed to create dynamic texture";
			return nullptr;
		}

		return make_shared<SDL2DynamicTexture>(tex);
	}

	bool SDL2RendererSDL::update_texture(DynamicTexture& texture, Surface& surface, Optional<Recti> rect)
	{
		if (const auto tex = dynamic_cast<SDL2DynamicTexture*>(&texture))
		{
#if 1
			SDL_Rect r;
			const auto prect = rect.has_value() ? &SDL2Utils::convert(rect.value(), r) : nullptr;

			// TODO: Pass surface pitch from format
			// The pixel data must be in the pixel format of the texture. Use SDL_QueryTexture() to query the pixel format of the texture.
			if (SDL_UpdateTexture(tex->handle(), prect, surface.data(), surface.size().x * 4) == 0)
				return true;

			UC_LOG_ERROR(_logger) << SDL_GetError();
			return false;
#else
			int pitch;
			void* buffer;
			if (SDL_LockTexture(tex->handle(), nullptr, &buffer, &pitch) == 0)
			{
				Memory::copy(buffer, surface.data(), surface.size().area() * 4);
				SDL_UnlockTexture(tex->handle());
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
			return false;
#endif
		}

		UC_LOG_ERROR(_logger) << "Invalid texture type";
		return false;
	}

	Shared<TargetTexture> SDL2RendererSDL::create_target_texture(const Vector2i& size)
	{
		auto tex = create_texture(size, SDL_TEXTUREACCESS_STREAMING);
		if (!tex)
		{
			UC_LOG_ERROR(_logger) << "Failed to create target texture";
			return nullptr;
		}

		return make_shared<SDL2TargetTexture>(tex);
	}

	bool SDL2RendererSDL::set_target(const Shared<TargetTexture>& texture)
	{
		if (const auto tex = std::dynamic_pointer_cast<SDL2TargetTexture>(texture))
		{
			if (SDL_SetRenderTarget(_renderer, tex->handle()) == 0)
			{
				_target = tex;
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
			return false;
		}

		UC_LOG_ERROR(_logger) << "Invalid texture type";
		return false;
	}

	const Shared<TargetTexture>& SDL2RendererSDL::get_target() const
	{
		return _target;
	}

	bool SDL2RendererSDL::begin_frame()
	{
		if (_display.size() != _size)
		{
			UC_LOG_DEBUG(_logger) << "Resized";
			update_size();
			update_scale();
			update_viewport();
		}

		set_clip(std::nullopt);
		set_draw_color(ColorConst4b::White);
		_draw_calls = 0;

		return true;
	}

	void SDL2RendererSDL::end_frame()
	{
		SDL_RenderPresent(_renderer);
	}

	void SDL2RendererSDL::clear(const Color4b& color)
	{
		SDL_SetRenderDrawColor(_renderer,
			color.r, color.g, color.b, color.a);
		SDL_RenderClear(_renderer);
	}

	// STATES /////////////////////////////////////////////////////////////////////
	void SDL2RendererSDL::set_viewport(const Optional<Recti>& rect)
	{
		_viewport = rect;
		if (_viewport.has_value())
		{
			SDL_Rect sdl_rect;
			SDL2Utils::convert(_viewport.value(), sdl_rect);
			SDL_RenderSetViewport(_renderer, &sdl_rect);
		}
		else
		{
			SDL_RenderSetViewport(_renderer, nullptr);
		}

		update_scale();
		update_logical_size();
	}

	void SDL2RendererSDL::set_scale(const Vector2f& value)
	{
		_scale = value;
		SDL_RenderSetScale(_renderer, _scale.x, _scale.y);

		update_viewport();
		update_logical_size();
	}

	void SDL2RendererSDL::set_logical_size(const Vector2i& size)
	{
		_logical_size = size;
		SDL_RenderSetLogicalSize(_renderer, _logical_size.x, _logical_size.y);

		update_scale();
		update_viewport();
	}

	void SDL2RendererSDL::set_clip(const Optional<Recti>& clip_rect)
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

	void SDL2RendererSDL::set_draw_color(const Color4b& color)
	{
		_color = color;
		SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
	}

	// DRAW POINTS ////////////////////////////////////////////////////////////////
	void SDL2RendererSDL::draw_point(const Vector2i& p)
	{
		SDL_RenderDrawPoint(_renderer, p.x, p.y);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_point_f(const Vector2f& p)
	{
		SDL_RenderDrawPointF(_renderer, p.x, p.y);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_points(const Vector2i* points, unsigned count)
	{
		SDL2Utils::convert(points, count, s_points);
		SDL_RenderDrawPoints(_renderer, s_points.data(), static_cast<int>(count));
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_points_f(const Vector2f* points, unsigned count)
	{
		SDL2Utils::convert(points, count, s_points_f);
		SDL_RenderDrawPointsF(_renderer, s_points_f.data(), static_cast<int>(count));
		_draw_calls++;
	}

	// DRAW LINES /////////////////////////////////////////////////////////////////
	void SDL2RendererSDL::draw_line(const Vector2i& p1, const Vector2i& p2)
	{
		SDL_RenderDrawLine(_renderer, p1.x, p1.y, p2.x, p2.y);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_line_f(const Vector2f& p1, const Vector2f& p2)
	{
		SDL_RenderDrawLineF(_renderer, p1.x, p1.y, p2.x, p2.y);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_poly_line(const Vector2i* points, unsigned count)
	{
		SDL2Utils::convert(points, count, s_points);
		SDL_RenderDrawLines(_renderer, s_points.data(), static_cast<int>(count));
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_poly_line_f(const Vector2f* points, unsigned count)
	{
		SDL2Utils::convert(points, count, s_points_f);
		SDL_RenderDrawLinesF(_renderer, s_points_f.data(), static_cast<int>(count));
		_draw_calls++;
	}

	// DRAW RECTS /////////////////////////////////////////////////////////////////
	void SDL2RendererSDL::draw_rect(const Recti& rect, bool filled)
	{
		SDL_Rect sdl_rect;
		SDL2Utils::convert(rect, sdl_rect);
		if (!filled) SDL_RenderDrawRect(_renderer, &sdl_rect);
		else SDL_RenderFillRect(_renderer, &sdl_rect);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_rect_f(const Rectf& rect, bool filled)
	{
		SDL_FRect sdl_rect;
		SDL2Utils::convert(rect, sdl_rect);
		if (!filled) SDL_RenderDrawRectF(_renderer, &sdl_rect);
		else SDL_RenderFillRectF(_renderer, &sdl_rect);
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_rects(const Recti* rects, unsigned count, bool filled)
	{
		SDL2Utils::convert(rects, count, s_rects);
		if (!filled)
			SDL_RenderDrawRects(_renderer, s_rects.data(), static_cast<int>(count));
		else
			SDL_RenderFillRects(_renderer, s_rects.data(), static_cast<int>(count));
		_draw_calls++;
	}

	void SDL2RendererSDL::draw_rects_f(const Rectf* rects, unsigned count, bool filled)
	{
		SDL2Utils::convert(rects, count, s_rects_f);
		if (!filled)
			SDL_RenderDrawRectsF(_renderer, s_rects_f.data(), static_cast<int>(count));
		else
			SDL_RenderFillRectsF(_renderer, s_rects_f.data(), static_cast<int>(count));
		_draw_calls++;
	}

	// DRAW TRIANGLES /////////////////////////////////////////////////////////////
	void SDL2RendererSDL::draw_triangles(
		const VertexColor2* vertices, unsigned num_vertices)
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

		const auto result = SDL_RenderGeometry(
			_renderer, nullptr,
			s_vertices.data(), static_cast<int>(num_vertices),
			nullptr, 0
		);
		if (result != 0)
			UC_LOG_ERROR(_logger) << SDL_GetError();

		_draw_calls++;
	}

	void SDL2RendererSDL::draw_triangles(const VertexTexColor2* vertices,
		unsigned num_vertices, const Texture* texture)
	{
		const auto tex = dynamic_cast<const SDL2BaseTexture*>(texture);
		const auto tex_handle = tex ? tex->handle() : nullptr;

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

		const auto result = SDL_RenderGeometry(
			_renderer, tex_handle,
			s_vertices.data(), static_cast<int>(num_vertices),
			nullptr, 0
		);
		if (result != 0)
			UC_LOG_ERROR(_logger) << SDL_GetError();

		_draw_calls++;
	}

	// COPY TEXTURE ///////////////////////////////////////////////////////////////
	bool SDL2RendererSDL::copy(const Shared<Texture>& texture,
		const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect)
	{
		if (const auto tex = std::dynamic_pointer_cast<SDL2BaseTexture>(texture))
		{
			SDL_Rect src, dst;
			const auto result = SDL_RenderCopy(_renderer, tex->handle(),
				src_rect.has_value() ? &SDL2Utils::convert(src_rect.value(), src) : nullptr,
				dst_rect.has_value() ? &SDL2Utils::convert(dst_rect.value(), dst) : nullptr);

			if (result == 0)
			{
				_draw_calls++;
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
		}

		return false;
	}

	bool SDL2RendererSDL::copy_f(const Shared<Texture>& texture,
		const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect)
	{
		if (const auto tex = std::dynamic_pointer_cast<SDL2BaseTexture>(texture))
		{
			SDL_Rect src;
			SDL_FRect dst;
			const auto result = SDL_RenderCopyF(_renderer, tex->handle(),
				src_rect.has_value() ? &SDL2Utils::convert(src_rect.value(), src) : nullptr,
				dst_rect.has_value() ? &SDL2Utils::convert(dst_rect.value(), dst) : nullptr);

			if (result == 0)
			{
				_draw_calls++;
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
		}

		return false;
	}

	bool SDL2RendererSDL::copy_ex(const Shared<Texture>& texture,
		const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
		Degrees angle, const Optional<Vector2i>& center, SDLRenderFlipFlags flip)
	{
		if (const auto tex = std::dynamic_pointer_cast<SDL2BaseTexture>(texture))
		{
			SDL_Point c;
			SDL_Rect src, dst;

			const auto result = SDL_RenderCopyEx(_renderer, tex->handle(),
				src_rect.has_value() ? &SDL2Utils::convert(src_rect.value(), src) : nullptr,
				dst_rect.has_value() ? &SDL2Utils::convert(dst_rect.value(), dst) : nullptr,
				(double)angle.value(),
				center.has_value() ? &SDL2Utils::convert(center.value(), c) : nullptr,
				convert_flip(flip)
			);

			if (result == 0)
			{
				_draw_calls++;
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
		}

		return false;
	}

	bool SDL2RendererSDL::copy_ex_f(const Shared<Texture>& texture,
		const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
		Degrees angle, const Optional<Vector2f>& center, SDLRenderFlipFlags flip)
	{
		if (const auto tex = std::dynamic_pointer_cast<SDL2BaseTexture>(texture))
		{
			SDL_FPoint c;
			SDL_Rect src;
			SDL_FRect dst;

			const auto result = SDL_RenderCopyExF(_renderer, tex->handle(),
				src_rect.has_value() ? &SDL2Utils::convert(src_rect.value(), src) : nullptr,
				dst_rect.has_value() ? &SDL2Utils::convert(dst_rect.value(), dst) : nullptr,
				(double)angle.value(),
				center.has_value() ? &SDL2Utils::convert(center.value(), c) : nullptr,
				convert_flip(flip)
			);

			if (result == 0)
			{
				_draw_calls++;
				return true;
			}

			UC_LOG_ERROR(_logger) << SDL_GetError();
		}

		return false;
	}

	Unique<SDL2RendererSDL> SDL2RendererSDL::create(Logger& logger, Display& display)
	{
		const auto sdl_display = dynamic_cast<SDL2Display*>(&display);
		if (!sdl_display)
		{
			UC_LOG_ERROR(logger) << "SDL Renderer can be created only from SDL2Display";
			return nullptr;
		}

		return std::make_unique<SDL2RendererSDL>(logger, *sdl_display);
	}

	// PROTECTED //////////////////////////////////////////////////////////////////
	void SDL2RendererSDL::update_size()
	{
		SDL_GetRendererOutputSize(_renderer, &_size.x, &_size.y);
	}

	void SDL2RendererSDL::update_scale()
	{
		SDL_RenderGetScale(_renderer, &_scale.x, &_scale.y);
	}

	void SDL2RendererSDL::update_viewport()
	{
		SDL_Rect r;
		SDL_RenderGetViewport(_renderer, &r);
		if (r.x == 0 && r.y == 0 && r.w == _size.x && r.h == _size.y)
			_viewport = std::nullopt;
		else
		{
			Recti viewport;
			SDL2Utils::convert(r, viewport);
			_viewport = viewport;
		}
	}

	void SDL2RendererSDL::update_logical_size()
	{
		SDL_RenderGetLogicalSize(_renderer, &_logical_size.x, &_logical_size.y);
	}

	SDL_Texture* SDL2RendererSDL::create_texture(const Vector2i& size, SDL_TextureAccess access) const
	{
		const auto tex = SDL_CreateTexture(_renderer,
			SDL_PIXELFORMAT_ABGR8888, access, size.x, size.y);
		if (!tex)
		{
			UC_LOG_ERROR(_logger) << SDL_GetError();
			return nullptr;
		}

		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
		return tex;
	}

	SDL_RendererFlip SDL2RendererSDL::convert_flip(SDLRenderFlipFlags flags)
	{
		int value = SDL_FLIP_NONE;
		if (flags.has(SDLRenderFlip::Horizontal))
			value |= SDL_FLIP_HORIZONTAL;
		if (flags.has(SDLRenderFlip::Vertical))
			value |= SDL_FLIP_VERTICAL;
		return static_cast<SDL_RendererFlip>(value);
	}
}

#endif