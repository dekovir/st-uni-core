#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class TextureLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<Texture>,
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
		ResourceLoaderTypePolicy::Multiple<DynamicTexture>,
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