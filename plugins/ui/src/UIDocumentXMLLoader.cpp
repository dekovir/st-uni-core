#include "unicore/ui/UIDocumentXMLLoader.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> UIDocumentXMLLoader::load_options(
		const Context& context, const LoggerOption& options)
	{
		const auto data = context.cache.load<XMLData>(context.path);
		if (!data)
			return nullptr;

		auto document = std::make_shared<UIDocument>(options.logger);

		if (!UIDocumentParseXML::parse(*data, *document))
			return nullptr;

		return document;
	}
}
#endif