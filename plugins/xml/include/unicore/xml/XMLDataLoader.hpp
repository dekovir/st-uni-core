#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	struct XMLLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		XMLLoadPolicy()
			: Extension({ ".xml" })
		{
		}
	};

	class XMLDataLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<XMLData>, XMLLoadPolicy>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}