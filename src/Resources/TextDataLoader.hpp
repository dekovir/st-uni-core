#pragma once
#include "unicore/TextData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct TextLoadPolicy : ResourceLoaderPolicy::ExtensionPolicy
	{
		TextLoadPolicy()
			: ExtensionPolicy({ L".txt" })
		{
		}
	};

	class TextDataLoader : public ResourceLoaderType<TextData, TextLoadPolicy>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderType)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class WTextDataLoader : public ResourceLoaderType<WTextData, TextLoadPolicy>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderType)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}