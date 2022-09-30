#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	struct XMLLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		XMLLoadPolicy()
			: Extension({ L".xml" })
		{
		}
	};

	class XMLDataLoader : public ResourceLoaderTyped<XMLData, XMLLoadPolicy>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}