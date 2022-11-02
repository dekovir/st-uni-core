#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document,
		const Optional<UINode>& parent, Logger* logger)
		: _inventory(inventory)
		, _document(document)
		, _logger(logger)
	{
		_inventory.on_add() += [&](auto index) { on_add(index); };
		_inventory.on_remove() += [&](auto index) { on_remove(index); };
		_inventory.on_changed() += [&](auto index) { on_change(index); };

		_money_text = _document.find_by_name("money", parent);

		_items_group = _document.find_by_name("item_group", parent);
		_item_template = _document.find_by_name("item_template", parent);
		_item_tooltip = _document.find_by_name("item_tooltip", parent);

		if (!_items_group) UC_LOG_ERROR(_logger) << "Group not found";
		if (!_item_template) UC_LOG_ERROR(_logger) << "Item template not found";

		apply_money(0);
	}

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: InventoryUI(inventory, document, std::nullopt, logger)
	{
	}

	void InventoryUI::on_add(InventoryIndex index)
	{
		if (!_items_group.has_value() || !_item_template.has_value())
			return;

		if (const auto new_node = _document.duplicate(_item_template.value(), _items_group.value()); new_node.has_value())
		{
			_item_nodes[index] = new_node.value();
			apply_item(new_node.value(), index);
		}
	}

	void InventoryUI::on_remove(InventoryIndex index)
	{
		const auto it = _item_nodes.find(index);
		if (it != _item_nodes.end())
		{
			_document.remove_node(it->second);
			_item_nodes.erase(it);
		}
	}

	void InventoryUI::on_change(InventoryIndex index)
	{
		if (const auto it = _item_nodes.find(index); it != _item_nodes.end())
		{
			apply_value(it->second, index);
			return;
		}

		UC_LOG_ERROR(_logger) << "Invalid index";
	}

	void InventoryUI::apply_money(UInt16 value)
	{
		if (_money_text.has_value())
		{
			const auto text = StringBuilder::format("Money: {}", value);
			_document.set_node_attribute(_money_text.value(), UIAttributeType::Text, text);
		}
	}

	void InventoryUI::apply_item(const UINode& node, InventoryIndex index)
	{
		const auto id = _inventory.get_index_id(index);

		const auto& item = *_inventory.database().get(id);
		UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

		_document.set_node_visible(node, true);

		if (const auto find = node.find_by_name("name"); find.valid())
		{
			_document.set_node_action(find, UIActionType::OnMouseEnter,
				[this, id] { apply_tooltip(id); });
			_document.set_node_action(find, UIActionType::OnMouseLeave,
				[this] { apply_tooltip(ItemId_Invalid); });
		}

		if (const auto find = node.find_by_name("icon"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Value, item.sprite);

		if (const auto find = node.find_by_name("action"); find.valid())
			_document.set_node_visible(find, item.is_wearable());

		if (const auto find = node.find_by_name("type"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, type_to_string(item.type));

		if (const auto find = node.find_by_name("price"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item.price);

		if (const auto find = node.find_by_name("weight"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, weight_to_string(item.weight));

		apply_value(node, index);
	}

	void InventoryUI::apply_value(const UINode& node, InventoryIndex index)
	{
		const auto id = _inventory.get_index_id(index);
		const auto& item = *_inventory.database().get(id);

		UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

		if (const auto find = node.find_by_name("name"); find.valid())
		{
			if (item.is_stackable())
			{
				const auto amount = _inventory.get_index_value(index);
				const auto str = StringBuilder::format("{} ({})", item.title, amount);
				_document.set_node_attribute(find, UIAttributeType::Text, str);
			}
			else _document.set_node_attribute(find, UIAttributeType::Text, item.title);
		}
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
		case ItemType::Shield: return "Shield";
		case ItemType::Armor: return "Armor";
		case ItemType::Accessory: return "Accessory";
		case ItemType::Consumable: return "Consumable";
		}
		return "Error";
	}

	String InventoryUI::weight_to_string(UInt16 weight)
	{
		const auto value = static_cast<float>(weight) / 1000.0f;
		return StringBuilder::format("{}", value);
	}
}