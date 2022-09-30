#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	struct XMLLoadPolicy : ResourceLoaderPolicy::ExtensionPolicy
	{
		XMLLoadPolicy()
			: ExtensionPolicy({ L".xml" })
		{
		}
	};

	class XMLDataLoader : public ResourceLoaderType<XMLData, XMLLoadPolicy>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}