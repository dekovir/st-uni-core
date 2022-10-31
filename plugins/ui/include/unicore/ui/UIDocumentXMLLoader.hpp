#pragma once
#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/xml/XMLDataLoader.hpp"

namespace unicore
{
	class UIDocumentXMLLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<UIDocument>, XMLLoadPolicy>
	{
		UC_OBJECT(UIDocumentXMLLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}
#endif