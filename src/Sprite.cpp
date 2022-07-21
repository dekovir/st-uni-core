#include "unicore/Sprite.hpp"
#include "unicore/Texture.hpp"

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
}