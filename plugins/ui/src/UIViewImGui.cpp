#include "unicore/ui/UIViewImGui.hpp"
#if defined(UNICORE_USE_IMGUI)
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

	Bool UIViewImGui::render_node(const UINode& node, Bool same_line)
	{
		const auto cached_info = get_info(node.index());
		if (!cached_info)
		{
			UC_ASSERT_ALWAYS_MSG("Invalid cache");
			return false;
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
		case UINodeType::None: return false;

		case UINodeType::Window:
			//bool_value = cached_info->value.get_bool();
			str = node.get_attribute(UIAttributeType::Text).get_string();

			if (ImGui::Begin(str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::End();
			return true;

		case UINodeType::Group:
			render_node_header(node, same_line);
			ImGui::BeginGroup();
			switch (get_layout(cached_info->value))
			{
			case UILayout::Horizontal:
				for (unsigned i = 0; i < children.size(); i++)
					render_node(children[i], i > 0);
				break;

			default:
				for (const auto& child : children)
					render_node(child);
				break;
			}
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case UINodeType::Text:
			str = node.get_attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			ImGui::Text("%s", str.c_str());
			render_node_footer(node);
			return true;

		case UINodeType::Button:
			str = node.get_attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			if (ImGui::Button(str.c_str()))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			render_node_footer(node);
			return true;

		case UINodeType::Input:
			str = cached_info->value.get_string();
			render_node_header(node, same_line);
			if (ImGui::InputText(id.c_str(), &str))
				_update_events.push_back({ node, UIEventType::ValueChanged, str });
			render_node_footer(node);
			return true;

		case UINodeType::Slider:
		{
			const auto value_min = node.get_attribute(UIAttributeType::MinValue).get_float(0);
			const auto value_max = node.get_attribute(UIAttributeType::MaxValue).get_float(1);
			const auto format = node.get_attribute(UIAttributeType::Text).get_string("%.2f");

			auto value = cached_info->value.get_float();
			render_node_header(node, same_line);
			if (ImGui::SliderFloat(id.c_str(), &value, value_min, value_max, format.c_str()))
			{
				cached_info->value = value;
				_update_events.push_back({ node, UIEventType::ValueChanged, value });
			}
			render_node_footer(node);
		}
		return true;

		case UINodeType::Toggle:
			bool_value = cached_info->value.get_bool();
			render_node_header(node, same_line);
			if (ImGui::Checkbox(id.c_str(), &bool_value))
			{
				cached_info->value = bool_value;
				_update_events.push_back({ node, UIEventType::ValueChanged, bool_value });
			}
			render_node_footer(node);
			return true;

		case UINodeType::Tooltip:
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				str = node.get_attribute(UIAttributeType::Text).get_string();
				if (!str.empty())
					ImGui::Text("%s", str.c_str());

				for (const auto& child : children)
					render_node(child);
				ImGui::EndTooltip();
			}
			return false;

		case UINodeType::Item:
			bool_value = cached_info->value.get_bool();
			str = node.get_attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			if (ImGui::Selectable(str.c_str(), bool_value))
			{
				cached_info->value = bool_value;
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			}
			render_node_footer(node);
			break;
		}

		return false;
	}

	void UIViewImGui::render_node_header(const UINode& node, Bool same_line)
	{
		if (same_line) ImGui::SameLine();
	}

	void UIViewImGui::render_node_footer(const UINode& node)
	{
		String tooltip;
		if (node.get_attribute(UIAttributeType::Tooltip).try_get_string(tooltip))
		{
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", tooltip.c_str());
		}
	}

	UIViewImGui::CachedInfo* UIViewImGui::get_info(UINodeIndex index)
	{
		const auto it = _cached.find(index);
		return it != _cached.end() ? &it->second : nullptr;
	}
}
#endif