#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/ui/UIDocumentParseXML.hpp"

namespace unicore
{
	static const auto xml = R"(
	<window>Inventory
		<text>Items</text>
		<group id="group">
		</group>
		<group id="item" value="2" visible="0">
			<text id="name">Name</text>
		</group>
	</window>
	)";

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: _inventory(inventory)
		, _document(document)
		, _logger(logger)
	{
		UIDocumentParseXML::parse(xml, _document, UINodeIndexInvalid, _logger);

		_inventory.on_add_item() += [&](auto index, auto& item) { on_add_item(index, item); };
		_inventory.on_remove_item() += [&](auto index, auto& item) { on_remove_item(index, item); };

		_items_group = _document.find_node_by_id("group");
		_items_template = _document.find_node_by_id("item");

		if (!_items_group) UC_LOG_ERROR(_logger) << "Group not found";
		if (!_items_template) UC_LOG_ERROR(_logger) << "Item template not found";
	}

	void InventoryUI::on_add_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;

		UC_LOG_DEBUG(_logger) << "Item " << item.title << " added";
	}

	void InventoryUI::on_remove_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;
	}
}