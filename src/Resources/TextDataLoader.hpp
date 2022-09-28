#pragma once
#include "unicore/TextData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class TextDataLoader : public ResourceLoaderT<TextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderT)
	public:
		TextDataLoader();
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};

	class WTextDataLoader : public ResourceLoaderT<WTextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderT)
	public:
		WTextDataLoader();
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}