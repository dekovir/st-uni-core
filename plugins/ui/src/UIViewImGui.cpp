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
			_hovered = ImGui::IsWindowHovered();
		}
		ImGui::End();

		for (const auto& evt : _update_events)
			_document->send_event(evt);
	}

	bool UIViewImGui::is_mouse_over() const
	{
		return _hovered;
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
		//UC_LOG_DEBUG(_logger) << "create_node " << node;

		CachedInfo info;
		info.id = StringBuilder::format("##{}", StringHelper::to_hex(node.index()));
		info.title = node.text().get_string() + info.id;

		_cached[node.index()] = info;

		for (const auto& child : node.get_children())
			on_create_node(child);
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttributeType type, const Optional<Variant>& value)
	{
		UIView::on_set_attribute(node, type, value);

		if (type == UIAttributeType::Text)
		{
			if (const auto info = get_info(node.index()))
				info->title = value->get_string() + info->id;
		}
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
		const auto& title = cached_info->title;

		List<UINode> children;
		node.get_children(children);

		const auto width = node.attribute(UIAttributeType::Width).get_float();
		const auto height = node.attribute(UIAttributeType::Height).get_float();

		Bool bool_value;
		Int int_value;
		String str;
		String32 str32;

		ImTextureID texture_id;
		ImVec2 size, uv0, uv1;

		switch (type)
		{
		case UINodeType::Group:
			render_node_header(node, same_line);
			ImGui::BeginGroup();
			switch (get_group_value(node.value()))
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
			if (get_texture(node.value(), texture_id, size, uv0, uv1))
			{
				const ImVec2 s = { width > 0 ? width : size.x, height > 0 ? height : size.y };

				render_node_header(node, same_line);
				ImGui::Image(texture_id, s, uv0, uv1);
				render_node_footer(node);
				return true;
			}
			break;

		case UINodeType::Button:
			render_node_header(node, same_line);
			if (ImGui::Button(title.c_str(), { width, height }))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			render_node_footer(node);
			return true;

		case UINodeType::Input:
			str = node.value().get_string();
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

			auto value = node.value().get_float();
			render_node_header(node, same_line);
			if (ImGui::SliderFloat(id.c_str(), &value, value_min, value_max, format.c_str()))
				_update_events.push_back({ node, UIEventType::ValueChanged, value });
			render_node_footer(node);
		}
		return true;

		case UINodeType::Toggle:
			bool_value = node.value().get_bool();
			render_node_header(node, same_line);
			if (ImGui::Checkbox(id.c_str(), &bool_value))
				_update_events.push_back({ node, UIEventType::ValueChanged, bool_value });
			render_node_footer(node);
			return true;

		case UINodeType::Tooltip:
			if (node.visible())
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

		case UINodeType::List:
			if (ImGui::BeginListBox(id.c_str()))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndListBox();
			}
			return true;

		case UINodeType::Item:
			bool_value = node.value().get_bool();
			render_node_header(node, same_line);
			if (ImGui::Selectable(title.c_str(), bool_value))
				_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
			render_node_footer(node);
			return true;

		case UINodeType::Tree:
			bool_value = node.value().get_bool();
			ImGui::SetNextItemOpen(bool_value);
			if (ImGui::TreeNode(title.c_str()))
			{
				if (!bool_value)
					_update_events.push_back({ node, UIEventType::ValueChanged, !bool_value });

				for (const auto& child : children)
					render_node(child);

				ImGui::TreePop();
			}
			else if (bool_value)
				_update_events.push_back({ node, UIEventType::ValueChanged, !bool_value });

			return true;

		case UINodeType::Combo:
			str = node.value().get_string();
			if (ImGui::BeginCombo(title.c_str(), str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndCombo();
			}
			return true;

		case UINodeType::Table:
			if (ImGui::BeginTable(id.c_str(), node.value().get_int(1),
				ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndTable();
			}
			return true;

		case UINodeType::TableHeader:
			str = node.text().get_string();

			int_value = ImGui::TableGetColumnIndex();

			ImGui::TableSetColumnIndex(int_value + 1);
			ImGui::PushItemWidth(50);
			ImGui::TableHeader(str.c_str());
			return true;

		case UINodeType::TableRow:
			ImGui::TableNextRow();
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();

			return true;

		case UINodeType::TableCell:
			ImGui::TableNextColumn();

			if (node.attribute(UIAttributeType::Text).try_get_string(str))
				ImGui::Text("%s", str.c_str());

			if (!children.empty())
			{
				ImGui::BeginGroup();
				for (const auto& child : children)
					render_node(child);
				ImGui::EndGroup();
			}

			return true;
		}

		return false;
	}

	void UIViewImGui::render_node_header(const UINode& node, Bool same_line)
	{
		if (same_line) ImGui::SameLine();
	}

	void UIViewImGui::render_node_footer(const UINode& node)
	{
		const bool is_item_hovered = ImGui::IsItemHovered();

		if (const auto info = get_info(node.index()))
		{
			if (info->mouse_over != is_item_hovered)
			{
				const auto event_type = info->mouse_over ? UIEventType::MouseLeave : UIEventType::MouseEnter;
				_update_events.push_back({ node, event_type, Variant::Empty });
				info->mouse_over = is_item_hovered;
				//UC_LOG_DEBUG(_logger) << "Mouse " << event_type << " at " << node;
			}
		}

		String tooltip;
		if (node.attribute(UIAttributeType::Tooltip).try_get_string(tooltip))
		{
			if (is_item_hovered)
				ImGui::SetTooltip("%s", tooltip.c_str());
		}
	}

	UIViewImGui::CachedInfo* UIViewImGui::get_info(UINode::IndexType index)
	{
		const auto it = _cached.find(index);
		return it != _cached.end() ? &it->second : nullptr;
	}

	bool UIViewImGui::get_texture(const Variant& value,
		ImTextureID& id, ImVec2& size, ImVec2& uv0, ImVec2& uv1)
	{
		if (const auto tex = value.get_object_cast<Texture>(); tex != nullptr)
		{
			id = tex.get();
			size = ImGuiConvert::convert(tex->size().cast<Float>());
			uv0 = { 0, 0 };
			uv1 = { 1 , 1 };
			return true;
		}

		if (const auto spr = value.get_object_cast<Sprite>(); spr != nullptr)
		{
			const auto tex_size = spr->texture()->size().cast<Float>();
			const auto rect = spr->rect().cast<Float>();

			id = spr->texture().get();
			size = { rect.w, rect.h };
			uv0 = { rect.x / tex_size.x, rect.y / tex_size.y };
			uv1 = { uv0.x + rect.w / tex_size.x, uv0.y + rect.h / tex_size.y };
			return true;
		}

		return false;
	}
}
#endif