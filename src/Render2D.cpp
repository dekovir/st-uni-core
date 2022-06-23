#include "unicore/Render2D.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	class Render2DTextureLoader : public ResourceLoaderT<Texture>
	{
	public:
		explicit Render2DTextureLoader(Render2D& render) : _render(render) {}

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override
		{
			const auto surface = context.cache.load<Surface>(context.path);
			return !surface ? nullptr : _render.create_texture(*surface);
		}

	protected:
		Render2D& _render;
	};

	Shared<Texture> Render2D::load_texture(const Shared<ReadStream>& stream)
	{
		const auto surface = load_surface(stream);
		return surface ? create_texture(*surface) : nullptr;
	}
}