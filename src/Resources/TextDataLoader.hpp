#pragma once
#include "unicore/TextData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct TextLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		TextLoadPolicy()
			: Extension({ L".txt" })
		{
		}
	};

	class TextDataLoader : public ResourceLoaderTyped<TextData, TextLoadPolicy>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class WTextDataLoader : public ResourceLoaderTyped<WTextData, TextLoadPolicy>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}