#include "unicore/Render.hpp"

namespace unicore
{
	Shared<Texture> Render::load_texture(const Shared<ReadStream>& stream)
	{
		const auto surface = load_surface(stream);
		return surface ? create_texture(*surface) : nullptr;
	}
}