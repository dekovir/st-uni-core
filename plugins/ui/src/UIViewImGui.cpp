#include "unicore/ui/UIViewImGui.hpp"
#if defined(UNICORE_USE_IMGUI)
#include "unicore/system/StringHelper.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/renderer/Sprite.hpp"
#include <imgui_stdlib.h>

namespace unicore
{
	static unsigned s_last_id = 0;

	UIViewImGui::UIViewImGui(ImGuiContext& context, Logger& logger)
		: _logger(logger)
		, _context(context)
		, _id(s_last_id)
	{
	}

	void UIViewImGui::render()
	{
		if (!_document) return;

		_update_events.clear();

		const auto str = StringBuilder::format("{}##{}", _title, _id);

		if (_set_pos.has_value())
		{
			ImGui::SetNextWindowPos(ImGuiConvert::convert(_set_pos.value()));
			_set_pos.reset();
		}

		if (_set_size.has_value())
		{
			ImGui::SetNextWindowSize(ImGuiConvert::convert(_set_size.value()));
			_set_size.reset();
		}

		ImGuiWindowFlags window_flags = 0;
		if (_flags.has(UIViewImGuiFlag::NoTiltebar))
			window_flags |= ImGuiWindowFlags_NoTitleBar;

		if (_flags.has(UIViewImGuiFlag::NoMove))
			window_flags |= ImGuiWindowFlags_NoMove;

		if (_flags.has(UIViewImGuiFlag::NoResize))
			window_flags |= ImGuiWindowFlags_NoResize;

		if (_flags.has(UIViewImGuiFlag::NoCollapse))
			window_flags |= ImGuiWindowFlags_NoCollapse;

		if (_flags.has(UIViewImGuiFlag::NoBackground))
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (ImGui::Begin(str.c_str(), nullptr, window_flags))
		{
			for (auto& node : _document->get_roots())
				render_node(node);

			_pos = ImGuiConvert::convert(ImGui::GetWindowPos());
			_size = ImGuiConvert::convert(ImGui::GetWindowSize());
		}
		ImGui::End();

		for (const auto& evt : _update_events)
			_document->send_event(evt);
	}

	void UIViewImGui::on_rebuild()
	{
		_cached.clear();

		if (_document)
		{
			for (const auto& node : _document->get_roots())
				on_create_node(node);
		}
	}

	void UIViewImGui::on_create_node(const UINode& node)
	{
		UC_LOG_DEBUG(_logger) << "create_node " << node.index();

		CachedInfo info;
		info.id = StringBuilder::format("##{}", StringHelper::to_hex(node.index()));
		_cached[node.index()] = info;
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttributeType type, const Optional<Variant>& value)
	{
		//UC_LOG_DEBUG(_logger) << "set_attribute " << node.index();
	}

	void UIViewImGui::on_set_action(const UINode& node,
		UIActionType type, const Optional<UIAction>& value)
	{
		//UC_LOG_DEBUG(_logger) << "set_action " << node.index();
	}

	Bool UIViewImGui::render_node(const UINode& node, Bool same_line)
	{
		if (!node.visible()) return false;

		const auto cached_info = get_info(node.index());
		if (!cached_info)
		{
			UC_ASSERT_ALWAYS_MSG("Invalid cache");
			return false;
		}

		const auto type = node.type();
		const auto& id = cached_info->id;
		const auto node_value = node.value();

		List<UINode> children;
		node.get_children(children);

		const auto width = node.attribute(UIAttributeType::Width).get_float();
		const auto height = node.attribute(UIAttributeType::Height).get_float();

		Bool bool_value;

		String str;
		String32 str32;

		switch (type)
		{
		case UINodeType::Group:
			render_node_header(node, same_line);
			ImGui::BeginGroup();
			switch (get_group_value(node_value))
			{
			case UIGroupValue::Horizontal:
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
			str = node.attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			ImGui::Text("%s", str.c_str());
			render_node_footer(node);
			return true;

		case UINodeType::Image:
			if (const auto tex = node_value.get_object_cast<Texture>(); tex != nullptr)
			{
				const auto size = tex->size().cast<Float>();
				const ImVec2 s = { width > 0 ? width : size.x, height > 0 ? height : size.y };

				render_node_header(node, same_line);
				ImGui::Image(tex.get(), s);
				render_node_footer(node);
				return true;
			}

			if (const auto spr = node_value.get_object_cast<Sprite>(); spr != nullptr)
			{
				const auto size = spr->texture()->size().cast<Float>();
				const auto rect = spr->rect().cast<Float>();

				const ImVec2 uv0{ rect.x / size.x, rect.y / size.y };
				const ImVec2 uv1{ uv0.x + rect.w / size.x, uv0.y + rect.h / size.y };
				const ImVec2 s = { width > 0 ? width : rect.w, height > 0 ? height : rect.h };

				render_node_header(node, same_line);
				ImGui::Image(spr->texture().get(), s, uv0, uv1);
				render_node_footer(node);
				return true;
			}
			break;

		case UINodeType::Button:
			str = node.attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			if (ImGui::Button(str.c_str(), { width, height }))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			render_node_footer(node);
			return true;

		case UINodeType::Input:
			str = node_value.get_string();
			render_node_header(node, same_line);
			if (ImGui::InputText(id.c_str(), &str))
				_update_events.push_back({ node, UIEventType::ValueChanged, str });
			render_node_footer(node);
			return true;

		case UINodeType::Slider:
		{
			const auto value_min = node.attribute(UIAttributeType::MinValue).get_float(0);
			const auto value_max = node.attribute(UIAttributeType::MaxValue).get_float(1);
			const auto format = node.attribute(UIAttributeType::Text).get_string("%.2f");

			auto value = node_value.get_float();
			render_node_header(node, same_line);
			if (ImGui::SliderFloat(id.c_str(), &value, value_min, value_max, format.c_str()))
				_update_events.push_back({ node, UIEventType::ValueChanged, value });
			render_node_footer(node);
		}
		return true;

		case UINodeType::Toggle:
			bool_value = node_value.get_bool();
			render_node_header(node, same_line);
			if (ImGui::Checkbox(id.c_str(), &bool_value))
				_update_events.push_back({ node, UIEventType::ValueChanged, bool_value });
			render_node_footer(node);
			return true;

		case UINodeType::Tooltip:
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				str = node.attribute(UIAttributeType::Text).get_string();
				if (!str.empty())
					ImGui::Text("%s", str.c_str());

				for (const auto& child : children)
					render_node(child);
				ImGui::EndTooltip();
			}
			return false;

		case UINodeType::Item:
			bool_value = node_value.get_bool();
			str = node.attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			if (ImGui::Selectable(str.c_str(), bool_value))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
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
		if (node.attribute(UIAttributeType::Tooltip).try_get_string(tooltip))
		{
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", tooltip.c_str());
		}
	}

	UIViewImGui::CachedInfo* UIViewImGui::get_info(UINode::IndexType index)
	{
		const auto it = _cached.find(index);
		return it != _cached.end() ? &it->second : nullptr;
	}
}
#endif