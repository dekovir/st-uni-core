#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class TTFontLoader : public ResourceLoaderT<TexturedFont>
	{
	public:
		TTFontLoader();

		UC_NODISCARD const TypeInfo* data_type() const override { return &typeid(TTFontOptions); }

		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}