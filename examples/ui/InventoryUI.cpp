#include "InventoryUI.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	InventoryUI::InventoryUI(Inventory& inventory, remoteui::Document& document,
		const remoteui::Element& parent, Logger* logger)
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

	InventoryUI::InventoryUI(Inventory& inventory, remoteui::Document& document, Logger* logger)
		: InventoryUI(inventory, document, remoteui::Element::Empty, logger)
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
			_document.set_node_attribute(_money_text, remoteui::Attribute::Text, value);
	}

	static std::pair<remoteui::Element, remoteui::Element> find_node(
		const remoteui::Element& node, StringView name)
	{
		if (const auto find = node.find_by_name(name); find.valid())
			return std::make_pair(find, find);

		return std::make_pair(
			node.find_by_name(String(name) + "_value"),
			node.find_by_name(String(name) + "_group")
		);
	}

	void InventoryUI::apply_item(const remoteui::Element& node, InventoryIndex index)
	{
		const auto id = _inventory.get_index_id(index);

		const auto& item = *_inventory.database().get(id);
		//UC_LOG_DEBUG(_logger) << "Apply item " << item.title << " to " << node;

		_document.set_node_hidden(node, false);

		if (const auto find = node.find_by_name("name"); find.valid())
		{
			const auto tmp_index = index;
			_document.subscribe_node(find, remoteui::UIActionType::OnMouseEnter,
				[this, tmp_index] { apply_tooltip(tmp_index); });
			_document.subscribe_node(find, remoteui::UIActionType::OnMouseLeave,
				[this] { apply_tooltip(InventoryIndex_Invalid); });
		}

		if (const auto find = node.find_by_name("icon"); find.valid())
			_document.set_node_attribute(find, remoteui::Attribute::Value, item.sprite);

		if (const auto find = node.find_by_name("price"); find.valid())
			_document.set_node_attribute(find, remoteui::Attribute::Text, item.price);

		if (const auto find = node.find_by_name("weight"); find.valid())
			_document.set_node_attribute(find, remoteui::Attribute::Text, weight_to_string(item.weight));

		apply_item_value(node, index);
	}

	void InventoryUI::apply_item_value(const remoteui::Element& node, InventoryIndex index)
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
				_document.set_node_attribute(find, remoteui::Attribute::Text, str);
			}
			else _document.set_node_attribute(find, remoteui::Attribute::Text, item.title);
		}
	}

	void InventoryUI::apply_tooltip(InventoryIndex index)
	{
		if (_item_tooltip.empty()) return;

		const auto id = _inventory.get_index_id(index);
		const auto item = _inventory.database().get(id);

		if (item == nullptr)
		{
			_document.set_node_hidden(_item_tooltip, true);
			return;
		}

		_document.set_node_hidden(_item_tooltip, false);

		if (const auto find = _item_tooltip.find_by_name("title"); find.valid())
			_document.set_node_attribute(find, remoteui::Attribute::Text, item->title);

		if (const auto find = _item_tooltip.find_by_name("type"); find.valid())
			_document.set_node_attribute(find, remoteui::Attribute::Text, Item::type_to_string(item->item_type));

		if (const auto [find, group] = find_node(_item_tooltip, "damage"); find.valid())
		{
			if (item->damage != RangeConsti::Zero)
			{
				const auto str = StringBuilder::format(U"{}-{}", item->damage.min, item->damage.max);

				_document.set_node_hidden(group, false);
				_document.set_node_attribute(find, remoteui::Attribute::Text, str);
			}
			else _document.set_node_hidden(group, true);
		}

		if (const auto [find, group] = find_node(_item_tooltip, "armor"); find.valid())
		{
			if (item->armor > 0)
			{
				const auto str = StringBuilder::format(U"+{}", item->armor);

				_document.set_node_hidden(group, false);
				_document.set_node_attribute(find, remoteui::Attribute::Text, str);
			}
			else _document.set_node_hidden(group, true);
		}

		if (const auto find = _item_tooltip.find_by_name("desc"); find.valid())
			_document.set_node_hidden(find, true);

		if (const auto [find, group] = find_node(_item_tooltip, "status"); find.valid())
		{
			_document.set_node_hidden(group, !item->has_status());
			const auto value = Math::inverse_lerp_numeric(_inventory.get_index_value(index));
			_document.set_node_attribute(find, remoteui::Attribute::Value, value);
		}
	}

	String InventoryUI::weight_to_string(UInt16 weight)
	{
		const auto value = static_cast<float>(weight) / 1000.0f;
		return StringBuilder::format("{}", value);
	}
}