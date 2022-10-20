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

	void UIViewImGui::update()
	{
		if (!_document) return;

		_update_events.clear();

		for (auto& node : _document->get_root_nodes())
			render_node(node);

		for (const auto& evt : _update_events)
			_document->send_event(evt);
	}

	void UIViewImGui::on_create_node(const UINode& node)
	{
		UC_LOG_DEBUG(_logger) << "create_node " << node.index();
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttributeType type, const Optional<UIAttributeValue>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_attribute " << node.index();
	}

	void UIViewImGui::on_set_action(const UINode& node,
		UIActionType type, const Optional<UIAction>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_action " << node.index();
	}

	void UIViewImGui::render_node(const UINode& node)
	{
		const auto type = node.type();

		List<UINode> children;
		node.get_children(children);

		const auto& attributes = node.attributes();
		String id, str;
		String32 str32;

		switch (type)
		{
		case UINodeType::None: return;

		case UINodeType::Window:
			str = node.get_string(UIAttributeType::Text, "Window");
			if (ImGui::Begin(str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::End();
			}
			break;

		case UINodeType::Group:
			id = make_id(node.index());
			if (ImGui::BeginChild(id.c_str()))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndChild();
			}
			break;

		case UINodeType::Text:
			if (node.try_get_string(UIAttributeType::Text, str))
			{
				ImGui::Text("%s", str.c_str());
			}
			else ImGui::Text("No text");
			break;

		case UINodeType::Button:
			str = node.get_string(UIAttributeType::Text);
			if (ImGui::Button(str.c_str()))
				_update_events.push_back({ node, UIEventType::ActionCall, UIActionType::OnClick });
			break;

		case UINodeType::Input:
		{
			const auto label = node.get_string(UIAttributeType::Text);
			str = node.get_string(UIAttributeType::Value);
			if (ImGui::InputText(label.c_str(), &str))
				_update_events.push_back({ node, UIEventType::ValueChanged, str });
		}
		break;

		case UINodeType::Slider:
		{
			const auto label = node.get_string(UIAttributeType::Text);
			const auto value_min = node.get_float(UIAttributeType::MinValue, 0);
			const auto value_max = node.get_float(UIAttributeType::MaxValue, 1);

			auto value = node.get_float(UIAttributeType::Value);

			if (ImGui::SliderFloat(label.c_str(), &value, value_min, value_max))
				_update_events.push_back({ node, UIEventType::ValueChanged, value });
		}
		break;
		}
	}

	String UIViewImGui::make_id(UINodeIndex index)
	{
		return StringHelper::to_hex(index.value);
	}
}