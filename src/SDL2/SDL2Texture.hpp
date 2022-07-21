#pragma once
#include "unicore/Texture.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2Texture : public Texture
	{
		UC_OBJECT(SDL2Texture, Texture)
	public:
		explicit SDL2Texture(SDL_Texture* context);

		~SDL2Texture() override
		{
			SDL_DestroyTexture(_context);
		}

		UC_NODISCARD size_t system_memory_use() const override { return sizeof(SDL2Texture); }
		UC_NODISCARD size_t video_memory_use() const override;

		UC_NODISCARD const Vector2i& size() const override { return _size; }

	protected:
		SDL_Texture* _context;
		Vector2i _size = VectorConst2i::Zero;

		void update_size();

		friend class RendererImpl;
	};
}
#endif