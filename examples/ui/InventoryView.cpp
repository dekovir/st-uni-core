#include "InventoryView.hpp"
#include "unicore/io/Logger.hpp"

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

		for (auto& node : _document->get_root_nodes())
			render_node(node);
	}

	void UIViewImGui::on_create_node(const UINode& node)
	{
		UC_LOG_DEBUG(_logger) << "create_node";
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttributeType type, const Optional<UIAttributeValue>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_attribute";
	}

	void UIViewImGui::on_set_action(const UINode& node,
		UIActionType type, const Optional<UIAction>& value)
	{
		UC_LOG_DEBUG(_logger) << "set_action";
	}

	void UIViewImGui::render_node(const UINode& node)
	{
		const auto type = node.type();

		List<UINode> children;
		node.get_children(children);

		const auto& attributes = node.attributes();
		Optional<UIAttributeValue> value;

		String str;
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
				_document->send_event({ node, UIEventType::ActionCall, {UIActionType::OnClick} });
			break;
		}
	}
}