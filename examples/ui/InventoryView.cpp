#include "InventoryView.hpp"
#include "unicore/system/StringHelper.hpp"
#include "unicore/io/Logger.hpp"
#include <imgui_stdlib.h>

namespace unicore
{
	UIViewImGui::UIViewImGui(ImGuiContext& context, Logger& logger)
		: _logger(logger)
		, _context(context)
	{
	}

	void UIViewImGui::render()
	{
		if (!_document) return;

		_update_events.clear();

		for (auto& node : _document->get_root_nodes())
			render_node(node);

		for (const auto& evt : _update_events)
			_document->send_event(evt);
	}

	void UIViewImGui::on_rebuild()
	{
		_cached.clear();

		if (_document)
		{
			for (const auto& node : _document->get_root_nodes())
				on_create_node(node);
		}
	}

	void UIViewImGui::on_create_node(const UINode& node)
	{
		UC_LOG_DEBUG(_logger) << "create_node " << node.index();

		CachedInfo info;
		info.id = StringBuilder::format("##{}", StringHelper::to_hex(node.index().value));
		info.value = node.get_attribute(UIAttributeType::Value);

		_cached[node.index()] = info;
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttributeType type, const Optional<Variant>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_attribute " << node.index();

		if (type == UIAttributeType::Value)
		{
			if (const auto it = _cached.find(node.index()); it != _cached.end())
			{
				if (value.has_value())
					it->second.value = value.value();
				else it->second.value = Variant();
			}
		}
	}

	void UIViewImGui::on_set_action(const UINode& node,
		UIActionType type, const Optional<UIAction>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_action " << node.index();
	}

	void UIViewImGui::render_node(const UINode& node)
	{
		const auto cached_info = get_info(node.index());
		if (!cached_info)
		{
			UC_ASSERT_ALWAYS_MSG("Invalid cache");
			return;
		}

		const auto type = node.type();
		const auto& id = cached_info->id;

		List<UINode> children;
		node.get_children(children);

		Bool bool_value;

		String str;
		String32 str32;

		switch (type)
		{
		case UINodeType::None: return;

		case UINodeType::Window:
			str = cached_info->value.get_string("Window");
			if (ImGui::Begin(str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::End();
			break;

		case UINodeType::Group:
			switch (get_layout(node.get_attribute(UIAttributeType::Layout)))
			{
			case UILayout::Horizontal:
				for (unsigned i = 0; i < children.size(); i++)
				{
					if (i > 0) ImGui::SameLine();
					render_node(children[i]);
				}
				break;

			default:
				for (const auto& child : children)
					render_node(child);
				break;
			}
			break;

		case UINodeType::Text:
			if (cached_info->value.try_get_string(str))
				ImGui::Text("%s", str.c_str());
			else ImGui::Text("No text");
			break;

		case UINodeType::Button:
			str = cached_info->value.get_string();
			if (ImGui::Button(str.c_str()))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			break;

		case UINodeType::Input:
		{
			str = cached_info->value.get_string();
			if (ImGui::InputText(id.c_str(), &str))
				_update_events.push_back({ node, UIEventType::ValueChanged, str });
		}
		break;

		case UINodeType::Slider:
		{
			const auto value_min = node.get_attribute(UIAttributeType::MinValue).get_float(0);
			const auto value_max = node.get_attribute(UIAttributeType::MaxValue).get_float(1);

			auto value = cached_info->value.get_float();

			if (ImGui::SliderFloat(id.c_str(), &value, value_min, value_max))
			{
				cached_info->value = value;
				_update_events.push_back({ node, UIEventType::ValueChanged, value });
			}
		}
		break;

		case UINodeType::Toggle:
		{
			bool_value = cached_info->value.get_bool();
			if (ImGui::Checkbox(id.c_str(), &bool_value))
			{
				cached_info->value = bool_value;
				_update_events.push_back({ node, UIEventType::ValueChanged, bool_value });
			}
		}
		break;
		}
	}

	UIViewImGui::CachedInfo* UIViewImGui::get_info(UINodeIndex index)
	{
		const auto it = _cached.find(index);
		return it != _cached.end() ? &it->second : nullptr;
	}
}