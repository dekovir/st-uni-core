#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class TextureLoader : public ResourceLoaderType<Texture>
	{
	public:
		explicit TextureLoader(Renderer& render);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};

	class DynamicTextureLoader : public ResourceLoaderType<DynamicTexture>
	{
	public:
		explicit DynamicTextureLoader(Renderer& render);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};
}