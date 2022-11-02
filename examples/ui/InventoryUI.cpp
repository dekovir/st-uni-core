#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/ui/UIDocumentParseXML.hpp"

namespace unicore
{
	static const auto xml = R"(
	<group>
		<text name="money" />
		<text>Items</text>
		<table name="item_group" value="4">
			<tr>
				<th>Icon</th>
				<th>Name</th>
				<th>Type</th>
				<th>Price</th>
			</tr>
		</table>
		<tr name="item_template" visible="0">
			<td><image name="icon" /></td>
			<td><item name="name">Name</item></td>
			<td><text name="type">Type</text></td>
			<td><text name="price">0</text></td>
		</tr>
	</group>
	)";

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: _inventory(inventory)
		, _document(document)
		, _logger(logger)
	{
		UIDocumentParseXML::parse(xml, _document, std::nullopt, _logger);

		_inventory.on_add_item() += [&](auto index, auto& item) { on_add_item(index, item); };
		_inventory.on_remove_item() += [&](auto index, auto& item) { on_remove_item(index, item); };

		_money_text = _document.find_by_name("money");
		_items_group = _document.find_by_name("item_group");
		_items_template = _document.find_by_name("item_template");

		if (!_items_group) UC_LOG_ERROR(_logger) << "Group not found";
		if (!_items_template) UC_LOG_ERROR(_logger) << "Item template not found";

		apply_money(_inventory.money());
	}

	void InventoryUI::on_change_money(UInt16 value)
	{
		apply_money(value);
	}

	void InventoryUI::on_add_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;

		UC_LOG_DEBUG(_logger) << "Item " << item.title << " added";

		if (const auto item_node = _document.duplicate(_items_template.value(), _items_group.value()); item_node.has_value())
			apply_item(item_node.value(), item);
	}

	void InventoryUI::on_remove_item(unsigned index, const Item& item)
	{
		if (!_items_group.has_value() || !_items_template.has_value())
			return;
	}

	void InventoryUI::apply_money(UInt16 value)
	{
		if (_money_text.has_value())
		{
			const auto text = StringBuilder::format("Money: {}", value);
			_document.set_node_attribute(_money_text.value(), UIAttributeType::Text, text);
		}
	}

	void InventoryUI::apply_item(const UINode& node, const Item& item)
	{
		_document.set_node_visible(node, true);

		if (const auto find = node.find_by_name("name"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item.title);

		if (const auto find = node.find_by_name("icon"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Value, item.sprite);

		if (const auto find = node.find_by_name("type"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, type_to_string(item.type));

		if (const auto find = node.find_by_name("price"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item.price);
	}

	StringView InventoryUI::type_to_string(ItemType type)
	{
		switch (type)
		{
		case ItemType::Weapon: return "Weapon";
		case ItemType::Armor: return "Armor";
		case ItemType::Accessory: return "Accessory";
		case ItemType::Consumable: return "Consumable";
		}
		return "Error";
	}
}