#pragma once
#include "unicore/Resource.hpp"
#include "unicore/xml/XMLNode.hpp"

namespace unicore
{
	class XMLData : public Resource
	{
	public:
		UC_NODISCARD size_t system_memory_use() const override { return sizeof(XMLData); }
		tinyxml2::XMLDocument doc;
	};
}