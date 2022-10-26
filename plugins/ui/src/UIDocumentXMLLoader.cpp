#include "unicore/ui/UIDocumentXMLLoader.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> UIDocumentXMLLoader::load(const Context& context)
	{
		const auto data = context.cache.load<XMLData>(context.path);
		if (!data)
			return nullptr;

		auto document = std::make_shared<UIDocument>();

		if (!UIDocumentParseXML::parse(*data, *document))
			return nullptr;

		return document;
	}
}
#endif