#pragma once
#include "unicore/ui/UIDocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/xml/XMLDataLoader.hpp"

namespace unicore
{
	class UIDocumentXMLLoader : public ResourceLoaderOptionsTyped<
		LoggerOption, ResourceLoaderTypePolicy::Single<UIDocument>, XMLLoadPolicy,
		ResourceLoaderOptionsPolicy::NullOrExact<LoggerOption>>
	{
		UC_OBJECT(UIDocumentXMLLoader, ResourceLoader)
	public:

		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const LoggerOption& options) override;
	};
}
#endif