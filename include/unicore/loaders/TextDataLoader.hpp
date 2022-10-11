#pragma once
#include "unicore/TextData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class TextDataLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<TextData>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class TextData16Loader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<TextData16>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextData16Loader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class TextData32Loader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<TextData32>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextData16Loader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}