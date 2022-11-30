#include "unicore/remoteui/DocumentXMLLoader.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/resource/ResourceCache.hpp"

namespace unicore::remoteui
{
	Shared<Resource> DocumentXMLLoader::load_options(
		const Context& context, const LoggerOption& options)
	{
		const auto data = context.cache.load<XMLData>(context.path);
		if (!data)
			return nullptr;

		auto document = std::make_shared<Document>(options.logger);

		if (!DocumentParseXML::parse(*data, *document))
			return nullptr;

		return document;
	}
}
#endif