#include "unicore/renderer/Sprite.hpp"
#include "unicore/renderer/Texture.hpp"

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

	size_t Sprite::get_system_memory_use() const
	{
		return sizeof(Recti) + _texture->get_system_memory_use();
	}

	size_t Sprite::get_used_resources(Set<Shared<Resource>>& resources)
	{
		if (_texture)
		{
			resources.insert(_texture);
			return 1;
		}

		return 0;
	}
}