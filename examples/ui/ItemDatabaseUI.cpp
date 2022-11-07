#include "ItemDatabaseUI.hpp"

namespace unicore
{
	ItemDatabaseUI::ItemDatabaseUI(ItemDatabase& database,
		UIDocument& document, const UINode& parent, Logger* logger)
		: _database(database)
		, _logger(logger)
	{
	}
}