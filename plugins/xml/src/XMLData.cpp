#include "unicore/xml/XMLData.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	Bool XMLData::parse(StringView xml, Logger* logger)
	{
		const auto result = doc.Parse(xml.data(), xml.size());
		if (result == tinyxml2::XML_SUCCESS)
			return true;

		UC_LOG_ERROR(logger) << doc.ErrorStr();
		return false;
	}

	XMLValue<tinyxml2::XMLElement> XMLData::get_root()
	{
		return XMLValue(doc.RootElement());
	}

	XMLValue<const tinyxml2::XMLElement> XMLData::get_root() const
	{
		return XMLValue(doc.RootElement());
	}
}