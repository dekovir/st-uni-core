#pragma once
#include "unicore/ResourceCache.hpp"
#if defined(UNICORE_USE_XML)
#include <tinyxml2.h>

namespace unicore
{
	class XMLData : public Resource
	{
	public:
		UC_NODISCARD size_t system_memory_use() const override { return sizeof(XMLData); }
		tinyxml2::XMLDocument doc;
	};

	class XMLDataLoader : public ResourceLoaderT<XMLData>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}
#endif