#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document,
		const UINode& parent, Logger* logger)
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

		if (_items_group.empty()) UC_LOG_ERROR(_logger) << "Group not found";
		if (_item_template.empty()) UC_LOG_ERROR(_logger) << "Item template not found";

		apply_money(0);
	}

	InventoryUI::InventoryUI(Inventory& inventory, UIDocument& document, Logger* logger)
		: InventoryUI(inventory, document, UINode::Empty, logger)
	{
	}

	void InventoryUI::on_add(InventoryIndex index)
	{
		if (_items_group.empty() || _item_template.empty())
			return;

		if (const auto new_node = _document.duplicate(_item_template, _items_group); !new_node.empty())
		{
			_item_nodes[index] = new_node;
			apply_item(new_node, index);
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
			apply_item_value(it->second, index);
			return;
		}

		UC_LOG_ERROR(_logger) << "Invalid index";
	}

	void InventoryUI::apply_money(UInt16 value)
	{
		if (!_money_text.empty())
		{
			const auto text = StringBuilder::format("Money: {}", value);
			_document.set_node_attribute(_money_text, UIAttributeType::Text, text);
		}
	}

	static std::pair<UINode, UINode> find_node(const UINode& node, StringView name)
	{
		if (const auto find = node.find_by_name(name); find.valid())
			return std::make_pair(find, UINode::Empty);

		return std::make_pair(
			node.find_by_name(String(name) + "_value"),
			node.find_by_name(String(name) + "_group")
		);
	}

	void InventoryUI::apply_item(const UINode& node, InventoryIndex index)
	{
		const auto id = _inventory.get_index_id(index);

		const auto& item = *_inventory.database().get(id);
		//UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

		_document.set_node_visible(node, true);

		if (const auto find = node.find_by_name("name"); find.valid())
		{
			const auto tmp_index = index;
			_document.set_node_action(find, UIActionType::OnMouseEnter,
				[this, tmp_index] { apply_tooltip(tmp_index); });
			_document.set_node_action(find, UIActionType::OnMouseLeave,
				[this] { apply_tooltip(InventoryIndex_Invalid); });
		}

		if (const auto find = node.find_by_name("icon"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Value, item.sprite);

		if (const auto find = node.find_by_name("price"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item.price);

		if (const auto find = node.find_by_name("weight"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, weight_to_string(item.weight));

		apply_item_value(node, index);
	}

	void InventoryUI::apply_item_value(const UINode& node, InventoryIndex index)
	{
		const auto id = _inventory.get_index_id(index);
		const auto& item = *_inventory.database().get(id);

		//UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

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

	void InventoryUI::apply_tooltip(InventoryIndex index)
	{
		if (_item_tooltip.empty()) return;

		const auto id = _inventory.get_index_id(index);
		const auto item = _inventory.database().get(id);

		if (item == nullptr)
		{
			_document.set_node_visible(_item_tooltip, false);
			return;
		}

		_document.set_node_visible(_item_tooltip, true);

		if (const auto find = _item_tooltip.find_by_name("title"); find.valid())
			_document.set_node_attribute(find, UIAttributeType::Text, item->title);

		if (const auto find = _item_tooltip.find_by_name("type"); find.valid())
		{
			const auto str = StringBuilder::format(U"Type: {}", type_to_string(item->type));
			_document.set_node_attribute(find, UIAttributeType::Text, str);
		}

		if (const auto find = _item_tooltip.find_by_name("damage"); find.valid())
		{
			if (item->damage != RangeConsti::Zero)
			{
				const auto str = StringBuilder::format(U"Damage: {}-{}", item->damage.min, item->damage.max);

				_document.set_node_visible(find, true);
				_document.set_node_attribute(find, UIAttributeType::Text, str);
			}
			else _document.set_node_visible(find, false);
		}

		if (const auto find = _item_tooltip.find_by_name("armor"); find.valid())
		{
			if (item->armor > 0)
			{
				const auto str = StringBuilder::format(U"Armor: +{}", item->armor);

				_document.set_node_visible(find, true);
				_document.set_node_attribute(find, UIAttributeType::Text, str);
			}
			else _document.set_node_visible(find, false);
		}

		if (const auto find = _item_tooltip.find_by_name("desc"); find.valid())
			_document.set_node_visible(find, false);

		if (const auto [find, group] = find_node(_item_tooltip, "status"); find.valid())
		{
			_document.set_node_visible(group, item->has_status());
			const auto value = Math::inverse_lerp_numeric(_inventory.get_index_value(index));
			_document.set_node_attribute(find, UIAttributeType::Value, value);
		}
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