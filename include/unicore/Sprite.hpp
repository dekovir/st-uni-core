#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class Sprite : public RenderResource
	{
	public:
		explicit Sprite(const Shared<Texture>& texture);
		Sprite(const Shared<Texture>& texture, const Recti& rect);

		UC_NODISCARD size_t system_memory_use() const override;
		UC_NODISCARD uint32_t video_memory_use() const override;

	protected:
		Shared<Texture> _texture;
		Recti _rect;
	};
}