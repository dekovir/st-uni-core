#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	XMLValue<tinyxml2::XMLElement> XMLData::get_root()
	{
		return XMLValue(doc.RootElement());
	}

	XMLValue<const tinyxml2::XMLElement> XMLData::get_root() const
	{
		return XMLValue(doc.RootElement());
	}
}