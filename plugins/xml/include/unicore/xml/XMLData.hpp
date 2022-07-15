#pragma once
#include "unicore/Resource.hpp"
#include "unicore/xml/XMLNode.hpp"

namespace unicore
{
	class XMLData : public Resource
	{
		UC_OBJECT(XMLData, Resource)
	public:
		UC_NODISCARD size_t system_memory_use() const override { return sizeof(XMLData); }
		tinyxml2::XMLDocument doc;

		XMLValue<tinyxml2::XMLElement> get_root()
		{
			return XMLValue(doc.RootElement());
		}

		XMLValue<const tinyxml2::XMLElement> get_root() const
		{
			return XMLValue(doc.RootElement());
		}
	};
}