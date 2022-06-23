#pragma once
#include "unicore/LogHelper.hpp"
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	namespace SDL2Utils
	{
		extern SDL_RWops* from_stream(ReadStream& stream);
		extern SDL_RWops* from_stream(const Shared<ReadStream>& stream);

		extern SDL_RWops* from_stream(const Shared<WriteStream>& stream);

		static SDL_Rect& convert(const Recti& src, SDL_Rect& dst)
		{
			dst = { src.x, src.y, src.w, src.h };
			return dst;
		}

		static SDL_FRect& convert(const Rectf& src, SDL_FRect& dst)
		{
			dst = { src.x, src.y, src.w, src.h };
			return dst;
		}

		static SDL_Point& convert(const Vector2i& src, SDL_Point& dst)
		{
			dst = { src.x, src.y };
			return dst;
		}

		static SDL_FPoint& convert(const Vector2f& src, SDL_FPoint& dst)
		{
			dst = { src.x, src.y };
			return dst;
		}

		static void convert(const Vector2i* src, size_t count, List<SDL_Point>& dst)
		{
			dst.resize(count);
			for (size_t i = 0; i < count; i++)
				convert(src[i], dst[i]);
		}

		static void convert(const Vector2f* src, size_t count, List<SDL_FPoint>& dst)
		{
			dst.resize(count);
			for (size_t i = 0; i < count; i++)
				convert(src[i], dst[i]);
		}

		static void convert(const Recti* src, size_t count, List<SDL_Rect>& dst)
		{
			dst.resize(count);
			for (size_t i = 0; i < count; i++)
				convert(src[i], dst[i]);
		}

		static void convert(const Rectf* src, size_t count, List<SDL_FRect>& dst)
		{
			dst.resize(count);
			for (size_t i = 0; i < count; i++)
				convert(src[i], dst[i]);
		}
	}

	LogHelper& operator<<(LogHelper& helper, const SDL_version& item);
}

#endif
