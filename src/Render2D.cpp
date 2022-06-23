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

	Render2D::Render2D(Logger& logger)
		: Render(logger)
	{
	}

	void Render2D::register_module(Context& context)
	{
		static Render2DTextureLoader loader(*this);

		Render::register_module(context);
		context.add_loader(loader);
	}

	void Render2D::unregister_module(Context& context)
	{
		Render::unregister_module(context);
	}
}