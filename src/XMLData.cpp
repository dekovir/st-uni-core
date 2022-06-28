#include "unicore/XMLData.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/Data.hpp"

namespace unicore
{
	bool XMLDataLoader::can_load_extension(WStringView ext) const
	{
		return ext == L".xml";
	}

	Shared<Resource> XMLDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto data = context.cache.load<BinaryData>(context.path);
		if (!data)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load BinaryData";
			return nullptr;
		}

		const auto xml = make_shared<XMLData>();
		if (xml->doc.Parse(static_cast<const char*>(data->data()), data->size()) == tinyxml2::XML_SUCCESS)
			return xml;

		return nullptr;
	}
}
#endif