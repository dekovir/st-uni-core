#include "unicore/Sprite.hpp"

namespace unicore
{
	Sprite::Sprite(const Shared<Texture>& texture)
		: _texture(texture), _rect(VectorConst2i::Zero, texture->size())
	{
	}

	Sprite::Sprite(const Shared<Texture>& texture, const Recti& rect)
		: _texture(texture), _rect(rect)
	{
	}

	size_t Sprite::system_memory_use() const
	{
		return sizeof(Recti) + _texture->system_memory_use();
	}

	uint32_t Sprite::video_memory_use() const
	{
		return _texture->video_memory_use();
	}
}