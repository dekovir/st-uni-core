#pragma once
#include "unicore/TextData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class TextDataLoader : public ResourceLoaderType<TextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderType)
	public:
		TextDataLoader();
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class WTextDataLoader : public ResourceLoaderType<WTextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderType)
	public:
		WTextDataLoader();
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}