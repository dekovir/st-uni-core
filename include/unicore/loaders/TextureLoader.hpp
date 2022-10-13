#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Texture.hpp"

namespace unicore
{
	class TextureLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<Texture>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextureLoader, ResourceLoader)
	public:
		explicit TextureLoader(Renderer& render);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};

	class DynamicTextureLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<DynamicTexture>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(DynamicTextureLoader, ResourceLoader)
	public:
		explicit DynamicTextureLoader(Renderer& render);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};
}