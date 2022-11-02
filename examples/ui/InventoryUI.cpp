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
		<tooltip name="item_tooltip" visible="0">
			<text name="title">title</text>
			<text name="desc">description</text>
		</tooltip>
	</group>
	)";

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: _inventory(inventory)
		, _document(document)
		, _logger(logger)
	{
		UIDocumentParseXML::parse(xml, _document, std::nullopt, _logger);

		_inventory.on_add_item() += [&](auto index, auto id) { on_add_item(index, id); };
		_inventory.on_remove_item() += [&](auto index, auto id) { on_remove_item(index, id); };

		_money_text = _document.find_by_name("money");

		_items_group = _document.find_by_name("item_group");
		_item_template = _document.find_by_name("item_template");
		_item_tooltip = _document.find_by_name("item_tooltip");

		if (!_items_group) UC_LOG_ERROR(_logger) << "Group not found";
		if (!_item_template) UC_LOG_ERROR(_logger) << "Item template not found";

		apply_money(_inventory.money());
	}

	void InventoryUI::on_change_money(UInt16 value)
	{
		apply_money(value);
	}

	void InventoryUI::on_add_item(unsigned index, ItemId id)
	{
		if (!_items_group.has_value() || !_item_template.has_value())
			return;

		if (const auto new_node = _document.duplicate(_item_template.value(), _items_group.value()); new_node.has_value())
			apply_item(new_node.value(), id);
	}

	void InventoryUI::on_remove_item(unsigned index, ItemId id)
	{
		if (!_items_group.has_value() || !_item_template.has_value())
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

	void InventoryUI::apply_item(const UINode& node, ItemId id)
	{
		const auto& item = *_inventory.database().get(id);
		UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

		_document.set_node_visible(node, true);

		if (const auto find = node.find_by_name("name"); find.valid())
		{
			_document.set_node_attribute(find, UIAttributeType::Text, item.title);

			_document.set_node_action(find, UIActionType::OnMouseEnter,
				[this, id] { apply_tooltip(id); });
			_document.set_node_action(find, UIActionType::OnMouseLeave,
				[this] { apply_tooltip(ItemId_Invalid); });
		}

		if (const auto find = node.find_by_name("icon"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Value, item.sprite);

		if (const auto find = node.find_by_name("type"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, type_to_string(item.type));

		if (const auto find = node.find_by_name("price"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item.price);
	}

	void InventoryUI::apply_tooltip(ItemId id)
	{
		if (!_item_tooltip.has_value()) return;

		if (const auto item = _inventory.database().get(id); item != nullptr)
		{
			_document.set_node_visible(_item_tooltip.value(), true);

			if (const auto find = _item_tooltip.value().find_by_name("title"); find.valid())
				_document.set_node_attribute(find, UIAttributeType::Text, item->title);

			if (const auto find = _item_tooltip.value().find_by_name("desc"); find.valid())
				_document.set_node_attribute(find, UIAttributeType::Text, "");
		}
		else _document.set_node_visible(_item_tooltip.value(), false);
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