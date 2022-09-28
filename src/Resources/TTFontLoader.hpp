#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class Renderer;

	class TTFontLoader : public ResourceLoaderT<TexturedFont>
	{
	public:
		explicit TTFontLoader(Renderer& renderer);

		UC_NODISCARD const TypeInfo* data_type() const override { return &typeid(TTFontOptions); }

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};
}