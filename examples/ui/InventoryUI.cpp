#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/ui/UIDocumentParseXML.hpp"

namespace unicore
{
	static const auto xml = R"(
	<window>Inventory
		<text>Items</text>
		<group name="item_group">
		</group>
		<group name="item_template" value="2" visible="0">
			<image name="icon" />
			<text name="name">Name</text>
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

		_items_group = _document.find_node_by_name_recurse("item_group");
		_items_template = _document.find_node_by_name_recurse("item_template");

		if (!_items_group) UC_LOG_ERROR(_logger) << "Group not found";
		if (!_items_template) UC_LOG_ERROR(_logger) << "Item template not found";
	}

	void InventoryUI::on_add_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;

		UC_LOG_DEBUG(_logger) << "Item " << item.title << " added";

		auto item_node = _items_template.value()
			.duplicate_at(_items_group.value().index());
		apply_item(item_node, item);
	}

	void InventoryUI::on_remove_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;
	}

	void InventoryUI::apply_item(UINode& node, const Item& item)
	{
		node.set_visible(true);

		if (auto find = node.find_child_by_name_recurse("name"); find.has_value())
			find.value().set_attribute(UIAttributeType::Text, item.title);

		if (auto find = node.find_child_by_name_recurse("icon"); find.has_value())
			find.value().set_attribute(UIAttributeType::Value, item.sprite);
	}
}