#pragma once
#include "unicore/ui/UINode.hpp"

namespace unicore
{
	class UIDocument
	{
	public:
		UC_NODISCARD UINode& root() { return _root; }
		UC_NODISCARD const UINode& root() const { return _root; }

		//void parse(StringView text);
	protected:
		UINode _root;
	};
}