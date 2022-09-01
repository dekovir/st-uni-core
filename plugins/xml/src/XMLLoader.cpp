#include "XMLLoader.hpp"
#include "unicore/TextData.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	XMLDataLoader::XMLDataLoader()
		: ResourceLoaderT({ L".xml" })
	{
	}

	Shared<Resource> XMLDataLoader::load(const Options& options)
	{
		const auto data = options.cache.load<TextData>(options.path);
		if (!data)
		{
			UC_LOG_ERROR(options.logger) << "Failed to load BinaryData";
			return nullptr;
		}

		const auto xml = make_shared<XMLData>();
		const auto& str = data->data();
		if (xml->doc.Parse(str.data(), str.size()) == tinyxml2::XML_SUCCESS)
			return xml;

		return nullptr;
	}
}