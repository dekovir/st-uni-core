#pragma once
#include "unicore/renderer/Texture.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2BaseTexture : public SDLValue<SDL_Texture>
	{
	public:
		explicit SDL2BaseTexture(SDL_Texture* handle)
			: SDLValue(handle)
		{}

		~SDL2BaseTexture() override
		{
			SDL_DestroyTexture(_handle);
		}

	protected:
		Vector2i _size = VectorConst2i::Zero;

		void update_size()
		{
			SDL_QueryTexture(_handle, nullptr, nullptr, &_size.x, &_size.y);
		}

		UC_NODISCARD size_t calc_video_memory() const
		{
			Uint32 format;
			if (SDL_QueryTexture(_handle, &format, nullptr, nullptr, nullptr) == 0)
			{
				const auto bpp = SDL_BYTESPERPIXEL(format);
				return (_size.area() * bpp) / 8;
			}

			return 0;
		}
	};

	class SDL2Texture : public Texture, public SDL2BaseTexture
	{
		UC_OBJECT(SDL2Texture, Texture)
	public:
		explicit SDL2Texture(SDL_Texture* handle);

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(SDL2Texture); }
		UC_NODISCARD size_t get_video_memory_use() const override { return calc_video_memory(); }

		UC_NODISCARD const Vector2i& size() const override { return _size; }
	};

	class SDL2DynamicTexture : public DynamicTexture, public SDL2BaseTexture
	{
		UC_OBJECT(SDL2DynamicTexture, DynamicTexture)
	public:
		explicit SDL2DynamicTexture(SDL_Texture* handle) : SDL2BaseTexture(handle) { update_size(); }

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(SDL2DynamicTexture); }
		UC_NODISCARD size_t get_video_memory_use() const override { return calc_video_memory(); }

		UC_NODISCARD const Vector2i& size() const override { return _size; }
	};

	class SDL2TargetTexture : public TargetTexture, public SDL2BaseTexture
	{
		UC_OBJECT(SDL2Texture, Texture)
	public:
		explicit SDL2TargetTexture(SDL_Texture* handle) : SDL2BaseTexture(handle) {}

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(SDL2TargetTexture); }
		UC_NODISCARD size_t get_video_memory_use() const override { return calc_video_memory(); }

		UC_NODISCARD const Vector2i& size() const override { return _size; }
	};
}
#endif