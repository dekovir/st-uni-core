#pragma once
#include "unicore/remoteui/Document.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/xml/XMLData.hpp"

namespace unicore::remoteui
{
	class DocumentParseXML
	{
	public:
		static Bool parse(StringView xml, Document& document,
			const Element& parent = Element::Empty, Logger* logger = nullptr);

		static Bool parse(const XMLData& data, Document& document,
			const Element& parent = Element::Empty, Logger* logger = nullptr);
	};
}
#endif