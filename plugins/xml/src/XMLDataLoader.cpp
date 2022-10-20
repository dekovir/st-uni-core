#include "unicore/xml/XMLDataLoader.hpp"
#include "unicore/resource/TextData.hpp"
#include "unicore/resource/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> XMLDataLoader::load(const Context& options)
	{
		const auto data = options.cache.load<TextData>(options.path);
		if (!data)
		{
			UC_LOG_ERROR(options.logger) << "Failed to load TextData";
			return nullptr;
		}

		const auto xml = make_shared<XMLData>();
		const auto& str = data->data();
		if (xml->doc.Parse(str.data(), str.size()) == tinyxml2::XML_SUCCESS)
			return xml;

		return nullptr;
	}
}