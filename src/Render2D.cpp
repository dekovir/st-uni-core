#include "unicore/Render2D.hpp"

namespace unicore
{
	Shared<Texture> Render2D::load_texture(const Shared<ReadStream>& stream)
	{
		const auto surface = load_surface(stream);
		return surface ? create_texture(*surface) : nullptr;
	}
}