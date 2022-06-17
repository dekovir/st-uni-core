#include "SDL2Render.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Stream.hpp"
#include "SDL2Utils.hpp"

namespace unicore
{
	SDL2Surface::SDL2Surface(SDL_Surface* context)
		: _context(context)
	{
		update_size();
	}

	void SDL2Surface::update_size()
	{
		_size.x = _context->w;
		_size.y = _context->h;
	}

	SDL2Texture::SDL2Texture(SDL_Texture* context)
		: _context(context)
	{
		update_size();
	}

	void SDL2Texture::update_size()
	{
		SDL_QueryTexture(_context, nullptr, nullptr, &_size.x, &_size.y);
	}

	SDL2Render::SDL2Render(const SDL2RenderSettings& settings)
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

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

		update_size();
	}

	SDL2Render::~SDL2Render()
	{
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
	}

	void SDL2Render::set_state(const RenderState& state)
	{
		_state.primitiveType = state.primitiveType;

		if (_state.texture != state.texture)
		{
			_state.texture = state.texture;
		}

		if (_state.clip_rect != state.clip_rect)
		{
			_state.clip_rect = state.clip_rect;
			if (_state.clip_rect.has_value())
			{
				SDL_Rect rect;
				SDL2Utils::convert(_state.clip_rect.value(), rect);
				SDL_RenderSetClipRect(_renderer, &rect);
			}
			else
			{
				SDL_RenderSetClipRect(_renderer, nullptr);
			}
		}
	}

	Shared<Surface> SDL2Render::load_surface(const Shared<ReadStream>& stream)
	{
		SDL_Surface* surface;
		if (const auto sdl_stream = std::dynamic_pointer_cast<SDL2Stream>(stream))
		{
			surface = SDL_LoadBMP_RW(sdl_stream->get_context(), SDL_FALSE);
		}
		else
		{
			const auto c = SDL2Utils::from_stream(stream);
			surface = c != nullptr ? SDL_LoadBMP_RW(c, SDL_TRUE) : nullptr;
		}

		return surface != nullptr ? make_shared<SDL2Surface>(surface) : nullptr;
	}

	Shared<Texture> SDL2Render::create_texture(Surface& surface)
	{
		if (const auto sdl = dynamic_cast<SDL2Surface*>(&surface))
		{
			if (auto texture = SDL_CreateTextureFromSurface(_renderer, sdl->_context))
				return make_shared<SDL2Texture>(texture);
		}

		return nullptr;
	}

	bool SDL2Render::begin_scene()
	{
		set_state({});
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

	void SDL2Render::draw_geometry(const Vertex* vertices, size_t num_vertices)
	{
		static std::vector<SDL_Vertex> points;
		const auto tex = std::dynamic_pointer_cast<SDL2Texture>(_state.texture);
		const auto tex_handle = tex ? tex->_context : nullptr;

		switch (_state.primitiveType)
		{
		case PrimitiveType::Points:
			for (size_t i = 0; i < num_vertices; i ++)
			{
				auto& v = vertices[i];
				SDL_SetRenderDrawColor(_renderer, v.col.r, v.col.g, v.col.b, v.col.a);
				SDL_RenderDrawPointF(_renderer, v.pos.x, v.pos.y);
			}
			break;

		case PrimitiveType::LineList:
			//SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
			for (size_t i = 0; i + 1 < num_vertices; i += 2)
			{
				auto& v0 = vertices[i];
				auto& v1 = vertices[i + 1];

				SDL_SetRenderDrawColor(_renderer, v0.col.r, v0.col.g, v0.col.b, v0.col.a);
				SDL_RenderDrawLineF(_renderer, v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y);
			}
			break;

		case PrimitiveType::TriangleList:
			points.resize(num_vertices);
			for (size_t i = 0; i < num_vertices; i++)
			{
				const auto& vertex = vertices[i];
				auto& [position, color, uv] = points[i];

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
				points.data(), static_cast<int>(num_vertices),
				nullptr, 0
			);
			break;

		default:
			UC_ASSERT_ALWAYS_MSG("Unimplemented primitive type");
			break;
		}
	}

	void SDL2Render::update_size()
	{
		SDL_GetRendererOutputSize(_renderer, &_size.x, &_size.y);
	}
}

#endif