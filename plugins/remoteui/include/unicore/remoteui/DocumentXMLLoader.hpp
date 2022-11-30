#pragma once
#include "unicore/remoteui/DocumentParseXML.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/xml/XMLDataLoader.hpp"

namespace unicore::remoteui
{
	class DocumentXMLLoader : public ResourceLoaderOptionsTyped<
		LoggerOption, ResourceLoaderTypePolicy::Single<Document>, XMLLoadPolicy,
		ResourceLoaderOptionsPolicy::NullOrExact<LoggerOption>>
	{
		UC_OBJECT(DocumentXMLLoader, ResourceLoader)
	public:

		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const LoggerOption& options) override;
	};
}
#endif