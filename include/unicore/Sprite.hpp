#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Resource.hpp"

namespace unicore
{
	class Texture;

	// TODO: Do we need virtual methods?
	class Sprite : public Resource
	{
		UC_OBJECT(Sprite, Resource)
	public:
		explicit Sprite(const Shared<Texture>& texture);
		Sprite(const Shared<Texture>& texture, const Recti& rect);

		UC_NODISCARD const Recti& rect() const { return _rect; }
		UC_NODISCARD const Shared<Texture>& texture() const { return _texture; }

		UC_NODISCARD size_t system_memory_use() const override;

	protected:
		Shared<Texture> _texture;
		Recti _rect;
	};
}