#pragma once
#include "unicore/ui/UIDocument.hpp"
#if defined(UNICORE_USE_XML)
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	class UIDocumentParseXML
	{
	public:
		static Bool parse(StringView xml, UIDocument& document,
			UINodeIndex parent = UINodeIndexInvalid, Logger* logger = nullptr);

		static Bool parse(const XMLData& data, UIDocument& document,
			UINodeIndex parent = UINodeIndexInvalid, Logger* logger = nullptr);
	};
}
#endif