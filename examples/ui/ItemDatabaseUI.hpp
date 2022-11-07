#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "ItemDatabase.hpp"

namespace unicore
{
	class ItemDatabaseUI
	{
	public:
		ItemDatabaseUI(ItemDatabase& database, UIDocument& document,
			const UINode& parent = UINode::Empty, Logger* logger = nullptr);

	protected:
		ItemDatabase& _database;
		Logger* _logger;
	};
}