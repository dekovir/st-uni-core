#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/resource/TextData.hpp"

namespace unicore
{
	class TextDataLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<TextData>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class TextData32Loader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<TextData32>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(TextData32Loader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}