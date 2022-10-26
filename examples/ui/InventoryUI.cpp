#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/ui/UIDocumentParseXML.hpp"

namespace unicore
{
	static const auto xml = R"(
	<group>
		<text name="money" />
		<text>Items</text>
		<group name="item_group">
		</group>
		<group name="item_template" value="2" visible="0">
			<image name="icon" />
			<item name="name">Name</item>
			<text name="type">Type</text>
			<text name="price">0</text>
		</group>
	</group>
	)";

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: _inventory(inventory)
		, _document(document)
		, _logger(logger)
	{
		UIDocumentParseXML::parse(xml, _document, UINodeIndexInvalid, _logger);

		_inventory.on_add_item() += [&](auto index, auto& item) { on_add_item(index, item); };
		_inventory.on_remove_item() += [&](auto index, auto& item) { on_remove_item(index, item); };

		_money_text = _document.find_node_by_name_recurse("money");
		_items_group = _document.find_node_by_name_recurse("item_group");
		_items_template = _document.find_node_by_name_recurse("item_template");

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

		auto item_node = _items_template.value()
			.duplicate_at(_items_group.value().index());
		apply_item(item_node, item);
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
			_money_text.value().set_attribute(UIAttributeType::Text, text);
		}
	}

	void InventoryUI::apply_item(UINode& node, const Item& item)
	{
		node.set_visible(true);

		if (auto find = node.find_child_by_name_recurse("name"); find.has_value())
			find.value().set_attribute(UIAttributeType::Text, item.title);

		if (auto find = node.find_child_by_name_recurse("icon"); find.has_value())
			find.value().set_attribute(UIAttributeType::Value, item.sprite);

		if (auto find = node.find_child_by_name_recurse("type"); find.has_value())
			find.value().set_attribute(UIAttributeType::Text, type_to_string(item.type));

		if (auto find = node.find_child_by_name_recurse("price"); find.has_value())
			find.value().set_attribute(UIAttributeType::Text, item.price);
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