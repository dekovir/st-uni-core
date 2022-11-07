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
		, _pos(0, 0)
		, _size(0, 0)
		, _id(s_last_id++)
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
		Float float_value;

		Vector2f vec2_value;
		Vector3f vec3_value;
		Color3f col3_value;
		Color4f col4_value;
		Rectf rect_value;

		String str;
		String32 str32;
		Rangef range_f;

		ImTextureID texture_id;
		ImVec2 size, uv0, uv1;

		switch (type)
		{
		case UINodeType::Group:
			render_node_header(node, same_line);
			switch (node.variant().get_enum<UIGroupVariant>())
			{
			case UIGroupVariant::Vertical:
				ImGui::BeginGroup();
				for (const auto& child : children)
					render_node(child);
				ImGui::EndGroup();
				break;

			case UIGroupVariant::Horizontal:
				ImGui::BeginGroup();
				for (unsigned i = 0; i < children.size(); i++)
					render_node(children[i], i > 0);
				ImGui::EndGroup();
				break;

			case UIGroupVariant::List:
				if (ImGui::BeginListBox(id.c_str(), { width, height }))
				{
					for (const auto& child : children)
						render_node(child);
					ImGui::EndListBox();
				}
				break;

			case UIGroupVariant::Popup:
				if (node.value().get_bool())
				{
					ImGui::OpenPopup(id.c_str());
					_document->set_node_attribute(node, UIAttributeType::Value, false);
				}

				if (ImGui::BeginPopup(id.c_str()))
				{
					for (const auto& child : children)
						render_node(child);

					ImGui::EndPopup();
				}
				break;
			}
			render_node_footer(node);
			return true;

		case UINodeType::Text:
			str = node.attribute(UIAttributeType::Text).get_string();

			render_node_header(node, same_line);
			ImGui::Text("%s", str.c_str());
			render_node_footer(node);
			return true;

		case UINodeType::Image:
			render_node_header(node, same_line);
			if (get_texture(node.value(), texture_id, size, uv0, uv1))
			{
				const ImVec2 s = { width > 0 ? width : size.x, height > 0 ? height : size.y };
				ImGui::Image(texture_id, s, uv0, uv1);
			}
			else ImGui::Image(nullptr, { width, height });
			render_node_footer(node);
			return true;

		case UINodeType::Input:
			render_node_header(node, same_line);
			switch (node.variant().get_enum<UIInputVariant>())
			{
			case UIInputVariant::Text:
				str = node.value().get_string();
				if (ImGui::InputText(id.c_str(), &str))
					_update_events.push_back({ node, UIEventType::ValueChanged, str });
				break;

			case UIInputVariant::TextArea:
				str = node.value().get_string();
				if (ImGui::InputTextMultiline(id.c_str(), &str, { width, height }))
					_update_events.push_back({ node, UIEventType::ValueChanged, str });
				break;

			case UIInputVariant::Toggle:
				bool_value = node.value().get_bool();
				if (ImGui::Checkbox(id.c_str(), &bool_value))
					_update_events.push_back({ node, UIEventType::ValueChanged, bool_value });
				break;

			case UIInputVariant::Radio:
				bool_value = node.value().get_bool();
				if (ImGui::RadioButton(id.c_str(), bool_value))
					_update_events.push_back({ node, UIEventType::ValueChanged, !bool_value });
				break;

			case UIInputVariant::Button:
				if (ImGui::Button(title.c_str(), { width, height }))
					_update_events.push_back({ node, UIEventType::Clicked, Variant::Empty });
				break;

			case UIInputVariant::Number:
				// TODO: Implement min/max
				// TODO: Implement ImGui::InputInt
				float_value = node.value().get_float();
				if (ImGui::InputFloat(id.c_str(), &float_value, node.attribute(UIAttributeType::StepValue).get_float(1)))
					_update_events.push_back({ node, UIEventType::ValueChanged, float_value });
				break;

			case UIInputVariant::Range:
				// TODO: Implement step
				range_f = {
					node.attribute(UIAttributeType::MinValue).get_float(0),
					node.attribute(UIAttributeType::MaxValue).get_float(1)
				};
				str = node.attribute(UIAttributeType::Text).get_string("%.2f");
				float_value = node.value().get_float();
				if (ImGui::SliderFloat(id.c_str(), &float_value, range_f.min, range_f.max, str.c_str()))
					_update_events.push_back({ node, UIEventType::ValueChanged, float_value });
				break;

			case UIInputVariant::Vector2:
				// TODO: Implement ImGui::InputInt2()
				vec2_value = node.value().get_vec2f();
				if (ImGui::InputFloat2(id.c_str(), &vec2_value.x))
					_update_events.push_back({ node, UIEventType::ValueChanged, vec2_value });
				break;

			case UIInputVariant::Vector3:
				// TODO: Implement ImGui::InputInt3()
				vec3_value = node.value().get_vec3f();
				if (ImGui::InputFloat3(id.c_str(), &vec3_value.x))
					_update_events.push_back({ node, UIEventType::ValueChanged, vec3_value });
				break;

			case UIInputVariant::Color3:
				col3_value = node.value().get_color3f();
				if (ImGui::ColorEdit3(id.c_str(), &col3_value.r))
					_update_events.push_back({ node, UIEventType::ValueChanged, col3_value });
				break;

			case UIInputVariant::Color4:
				col4_value = node.value().get_color4f();
				if (ImGui::ColorEdit4(id.c_str(), &col4_value.r))
					_update_events.push_back({ node, UIEventType::ValueChanged, col4_value });
				break;
			}

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
			render_node_header(node, same_line);
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
			render_node_footer(node);
			return true;

		case UINodeType::Combo:
			str = node.value().get_string();
			render_node_header(node, same_line);
			if (ImGui::BeginCombo(title.c_str(), str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndCombo();
			}
			render_node_footer(node);
			return true;

		case UINodeType::Table:
			render_node_header(node, same_line);
			if (ImGui::BeginTable(id.c_str(), node.value().get_int(1),
				ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndTable();
			}
			render_node_footer(node);
			return true;

		case UINodeType::TableHeader:
			str = node.text().get_string();

			ImGui::TableNextColumn();
			render_node_header(node, same_line);
			ImGui::TableHeader(str.c_str());
			render_node_footer(node);
			return true;

		case UINodeType::TableRow:
			ImGui::TableNextRow();
			render_node_header(node, same_line);
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();
			render_node_footer(node);

			return true;

		case UINodeType::TableCell:
			ImGui::TableNextColumn();

			render_node_header(node, same_line);
			ImGui::BeginGroup();
			if (node.attribute(UIAttributeType::Text).try_get_string(str))
				ImGui::Text("%s", str.c_str());

			if (!children.empty())
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::EndGroup();
			render_node_footer(node);

			return true;

		case UINodeType::Progress:
			range_f = {
				node.attribute(UIAttributeType::MinValue).get_float(0),
				node.attribute(UIAttributeType::MaxValue).get_float(1)
			};
			float_value = Math::inverse_lerp(range_f.min, range_f.max, node.value().get_float());
			render_node_header(node, same_line);
			if (node.attribute(UIAttributeType::Text).try_get_string(str))
				ImGui::ProgressBar(float_value, { width, height }, str.c_str());
			else ImGui::ProgressBar(float_value, { width, height });
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