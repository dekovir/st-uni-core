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
		info.id = StringBuilder::format("##{}", StringHelper::to_hex(node.index().value));
		info.title = node.text().get_string() + info.id;

		_cached[node.index()] = info;

		for (const auto& child : node.get_children())
			on_create_node(child);
	}

	void UIViewImGui::on_set_attribute(const UINode& node,
		UIAttribute type, const Optional<Variant>& value)
	{
		UIView::on_set_attribute(node, type, value);

		if (type == UIAttribute::Text)
		{
			if (const auto info = get_info(node.index()))
				info->title = value->get_string() + info->id;
		}
	}

	Bool UIViewImGui::render_node(const UINode& node, LayoutOption layout_option)
	{
		if (node.hidden()) return false;

		const auto cached_info = get_info(node.index());
		if (!cached_info)
		{
			UC_ASSERT_ALWAYS_MSG("Invalid cache");
			return false;
		}

		const auto tag = node.tag();
		const auto& title = cached_info->title;

		Bool bool_value;

		String str;
		String32 str32;

		switch (tag)
		{
		case UINodeTag::Group: // GROUP ////////////////////////////////////////////
			return render_group(*cached_info, node, layout_option);

		case UINodeTag::Visual: // VISUAL //////////////////////////////////////////
			return render_visual(*cached_info, node, layout_option);

		case UINodeTag::Input: // INPUT ////////////////////////////////////////////
			return render_input(*cached_info, node, layout_option);
		}

		return false;
	}

	Bool UIViewImGui::render_group(const CachedInfo& info,
		const UINode& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto& title = info.title;

		const auto width = node.get(UIAttribute::Width).get_float();
		const auto height = node.get(UIAttribute::Height).get_float();

		Bool bool_value;
		String str;
		String32 str32;

		List<UINode> children;
		node.get_children(children);

		switch (node.type().get_enum<UIGroupType>())
		{
		case UIGroupType::Vertical: // VERTICAL ////////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case UIGroupType::Horizontal: // HORIZONTAL ////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::BeginGroup();
			for (unsigned i = 0; i < children.size(); i++)
			{
				const auto option = i > 0
					? LayoutOption::SameLine : LayoutOption::None;
				render_node(children[i], option);
			}
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case UIGroupType::Child: // CHILD //////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginChild(id.c_str(), { width, height }, false, ImGuiWindowFlags_AlwaysAutoResize))
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::EndChild();
			render_node_footer(node);
			return true;

		case UIGroupType::List: // LIST ////////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginListBox(id.c_str(), { width, height }))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndListBox();
			}
			render_node_footer(node);
			return true;

		case UIGroupType::Tree: // TREE ////////////////////////////////////////////
			bool_value = node.value().get_bool();
			ImGui::SetNextItemOpen(bool_value);
			render_node_header(node, layout_option);
			if (ImGui::TreeNode(title.c_str()))
			{
				if (!bool_value)
					_update_events.push_back({ node, UIActionType::OnChange, !bool_value });

				for (const auto& child : children)
					render_node(child);

				ImGui::TreePop();
			}
			else if (bool_value)
				_update_events.push_back({ node, UIActionType::OnChange, !bool_value });
			render_node_footer(node);
			return true;

		case UIGroupType::Combo: // COMBO //////////////////////////////////////////
			str = node.value().get_string();
			render_node_header(node, layout_option);
			if (ImGui::BeginCombo(title.c_str(), str.c_str()))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndCombo();
			}
			render_node_footer(node);
			return true;

		case UIGroupType::Flex: // FLEX ////////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginListBox(id.c_str(), { width, height }))
			{
				for (const auto& child : children)
					render_node(child, LayoutOption::SameLineFlex);
				ImGui::EndListBox();
			}
			render_node_footer(node);
			return true;

		case UIGroupType::Table: // TABLE //////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginTable(id.c_str(), node.value().get_int(1),
				ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndTable();
			}
			render_node_footer(node);
			return true;

		case UIGroupType::TableHeader: // TABLE HEADER /////////////////////////////
			render_node_header(node, layout_option);
			str = node.text().get_string();
			ImGui::TableNextColumn();
			ImGui::TableHeader(str.c_str());
			render_node_footer(node);
			return true;

		case UIGroupType::TableRow: // TABLE ROW ///////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::TableNextRow();
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case UIGroupType::TableCell: // TABLE CELL /////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::TableNextColumn();
			ImGui::BeginGroup();
			if (node.get(UIAttribute::Text).try_get_string(str))
				ImGui::Text("%s", str.c_str());

			if (!children.empty())
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case UIGroupType::Popup: // POPUP //////////////////////////////////////////
			if (node.value().get_bool())
			{
				ImGui::OpenPopup(id.c_str());
				_document->set_node_attribute(node, UIAttribute::Value, false);
			}

			if (ImGui::BeginPopup(id.c_str()))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndPopup();
			}
			break;

		case UIGroupType::Tooltip: // TOOLTIP //////////////////////////////////////
			ImGui::BeginTooltip();

			str = node.get(UIAttribute::Text).get_string();
			if (!str.empty())
				ImGui::Text("%s", str.c_str());

			for (const auto& child : children)
				render_node(child);
			ImGui::EndTooltip();
			break;

		case UIGroupType::Modal: // MODAL //////////////////////////////////////////
			if (node.value().get_bool())
			{
				ImGui::OpenPopup(id.c_str());
				_document->set_node_attribute(node, UIAttribute::Value, false);
			}

			if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndPopup();
			}
			break;
		}

		return true;
	}

	Bool UIViewImGui::render_visual(const CachedInfo& info,
		const UINode& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto width = node.get(UIAttribute::Width).get_float();
		const auto height = node.get(UIAttribute::Height).get_float();

		Float float_value;
		String str;
		String32 str32;
		Rangef range_f;
		Color4f col4_value;

		ImTextureID texture_id;
		ImVec2 size, uv0, uv1;

		switch (node.type().get_enum<UIVisualType>())
		{
		case UIVisualType::Text: // TEXT ///////////////////////////////////////////
			str = node.get(UIAttribute::Text).get_string();

			render_node_header(node, layout_option);
			ImGui::Text("%s", str.c_str());
			render_node_footer(node);
			return true;

		case UIVisualType::Color: // COLOR /////////////////////////////////////////
			col4_value = node.value().get_color4f();

			render_node_header(node, layout_option);
			if (ImGui::ColorButton(id.c_str(), ImGuiConvert::convert_color(col4_value)))
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });

			render_node_footer(node);
			return true;

		case UIVisualType::Image: // IMAGE /////////////////////////////////////////
			render_node_header(node, layout_option);
			if (get_texture(node.value(), texture_id, size, uv0, uv1))
			{
				const ImVec2 s = { width > 0 ? width : size.x, height > 0 ? height : size.y };
				ImGui::Image(texture_id, s, uv0, uv1);
			}
			else ImGui::Image(nullptr, { width, height });
			render_node_footer(node);
			return true;

		case UIVisualType::Progress: // PROGRESS ///////////////////////////////////
			range_f = {
				node.get(UIAttribute::Min).get_float(0),
				node.get(UIAttribute::Max).get_float(1)
			};
			float_value = Math::inverse_lerp(
				range_f.min, range_f.max, node.value().get_float());
			render_node_header(node, layout_option);
			if (node.get(UIAttribute::Text).try_get_string(str))
				ImGui::ProgressBar(float_value, { width, height }, str.c_str());
			else ImGui::ProgressBar(float_value, { width, height });
			render_node_footer(node);
			return true;

		case UIVisualType::Separator: // SEPARATOR /////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::Separator();
			render_node_footer(node);
			return true;

		case UIVisualType::Bullet: // BULLET ///////////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::Bullet();
			render_node_footer(node);
			return true;
		}

		return false;
	}

	Bool UIViewImGui::render_input(const CachedInfo& info,
		const UINode& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto& title = info.title;

		const auto width = node.get(UIAttribute::Width).get_float();
		const auto height = node.get(UIAttribute::Height).get_float();

		Bool bool_value;
		Int int_value;
		Float float_value;

		Vector2f vec2_value;
		Vector3f vec3_value;
		Color3f col3_value;
		Color4f col4_value;

		String str;
		String32 str32;
		Rangef range_f;
		Rangei range_i;

		ImTextureID texture_id;
		ImVec2 size, uv0, uv1;

		render_node_header(node, layout_option);
		switch (node.type().get_enum<UIInputType>())
		{
		case UIInputType::Text: // TEXT ////////////////////////////////////////////
			render_node_header(node, layout_option);
			str = node.value().get_string();
			if (ImGui::InputText(id.c_str(), &str))
				_update_events.push_back({ node, UIActionType::OnChange, str });
			render_node_footer(node);
			break;

		case UIInputType::TextArea: // TEXTAREA ////////////////////////////////////
			render_node_header(node, layout_option);
			str = node.value().get_string();
			if (ImGui::InputTextMultiline(id.c_str(), &str, { width, height }))
				_update_events.push_back({ node, UIActionType::OnChange, str });
			render_node_footer(node);
			break;

		case UIInputType::Toggle: // TOGGLE ////////////////////////////////////////
			render_node_header(node, layout_option);
			bool_value = node.value().get_bool();
			if (ImGui::Checkbox(id.c_str(), &bool_value))
				_update_events.push_back({ node, UIActionType::OnChange, bool_value });
			render_node_footer(node);
			break;

		case UIInputType::Radio: // RADIO //////////////////////////////////////////
			render_node_header(node, layout_option);
			bool_value = node.value().get_bool();
			if (ImGui::RadioButton(id.c_str(), bool_value))
			{
				_update_events.push_back({ node, UIActionType::OnChange, !bool_value });
				ImGui::CloseCurrentPopup();
			}
			render_node_footer(node);
			break;

		case UIInputType::Button: // BUTTON ////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::Button(title.c_str(), { width, height }))
			{
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
				ImGui::CloseCurrentPopup();
			}
			render_node_footer(node);
			break;

		case UIInputType::Item: // ITEM ////////////////////////////////////////////
			bool_value = node.value().get_bool();
			render_node_header(node, layout_option);
			if (ImGui::Selectable(title.c_str(), bool_value))
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
			render_node_footer(node);
			break;

		case UIInputType::Image: // IMAGE //////////////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::PushID(id.c_str());
			if (get_texture(node.value(), texture_id, size, uv0, uv1))
			{
				const ImVec2 s = {
					width > 0 ? width : size.x,
					height > 0 ? height : size.y
				};
				if (ImGui::ImageButton(texture_id, s, uv0, uv1))
				{
					_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
					ImGui::CloseCurrentPopup();
				}
			}
			else
			{
				if (ImGui::ImageButton(nullptr, { width, height }))
				{
					_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::PopID();
			render_node_footer(node);
			return true;

		case UIInputType::Integer: // INTEGER //////////////////////////////////////
			range_i = {
				node.get(UIAttribute::Min).get_int(std::numeric_limits<Int>::min()),
				node.get(UIAttribute::Max).get_int(std::numeric_limits<Int>::max())
			};

			int_value = node.value().get_int();
			if (ImGui::InputInt(id.c_str(), &int_value,
				node.get(UIAttribute::Step).get_int(0)))
			{
				int_value = range_i.clamp(int_value);
				_update_events.push_back({ node, UIActionType::OnChange, int_value });
			}
			break;

		case UIInputType::Float: // FLOAT //////////////////////////////////////////
			range_f = {
				node.get(UIAttribute::Min).get_float(-std::numeric_limits<Float>::max()),
				node.get(UIAttribute::Max).get_float(+std::numeric_limits<Float>::max())
			};
			float_value = node.value().get_float();
			if (ImGui::InputFloat(id.c_str(), &float_value,
				node.get(UIAttribute::Step).get_float(0)))
			{
				float_value = range_f.clamp(float_value);
				_update_events.push_back({ node, UIActionType::OnChange, float_value });
			}

			break;

		case UIInputType::RangeI: // RANGEI ////////////////////////////////////////
			// TODO: Implement step
			range_i = {
				node.get(UIAttribute::Min).get_int(0),
				node.get(UIAttribute::Max).get_int(100)
			};
			str = node.get(UIAttribute::Text).get_string("%d");
			int_value = node.value().get_int();
			if (ImGui::SliderInt(id.c_str(), &int_value,
				range_i.min, range_i.max, str.c_str()))
				_update_events.push_back({ node, UIActionType::OnChange, int_value });
			break;

		case UIInputType::RangeF: // RANGEF ////////////////////////////////////////
			// TODO: Implement step
			range_f = {
				node.get(UIAttribute::Min).get_float(0),
				node.get(UIAttribute::Max).get_float(1)
			};
			str = node.get(UIAttribute::Text).get_string("%.2f");
			float_value = node.value().get_float();
			if (ImGui::SliderFloat(id.c_str(), &float_value,
				range_f.min, range_f.max, str.c_str()))
				_update_events.push_back({ node, UIActionType::OnChange, float_value });
			break;

		case UIInputType::Color3: // COLOR3 ////////////////////////////////////////
			col3_value = node.value().get_color3f();
			if (ImGui::ColorEdit3(id.c_str(), &col3_value.r))
				_update_events.push_back({ node, UIActionType::OnChange, col3_value });
			break;

		case UIInputType::Color4: // COLOR4 ////////////////////////////////////////
			col4_value = node.value().get_color4f();
			if (ImGui::ColorEdit4(id.c_str(), &col4_value.r))
				_update_events.push_back({ node, UIActionType::OnChange, col4_value });
			break;
		}

		render_node_footer(node);
		return true;
	}

	void UIViewImGui::render_node_header(
		const UINode& node, LayoutOption layout_option)
	{
		switch (layout_option)
		{
		case LayoutOption::None:
			break;

		case LayoutOption::SameLine:
			ImGui::SameLine();
			break;

		case LayoutOption::SameLineFlex:
			const auto size = ImGui::GetItemRectSize();
			const ImGuiStyle& style = ImGui::GetStyle();
			const float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			const float last_button_x2 = ImGui::GetItemRectMax().x;
			// Expected position if next button was on same line
			const float next_button_x2 = last_button_x2 + style.ItemSpacing.x + size.x;
			if (next_button_x2 < window_visible_x2)
				ImGui::SameLine();
			break;
		}
	}

	void UIViewImGui::render_node_footer(const UINode& node)
	{
		const bool is_item_hovered = ImGui::IsItemHovered();

		if (const auto info = get_info(node.index()))
		{
			if (info->mouse_over != is_item_hovered)
			{
				const auto event_type = info->mouse_over
					? UIActionType::OnMouseLeave : UIActionType::OnMouseEnter;
				_update_events.push_back({ node, event_type, Variant::Empty });
				info->mouse_over = is_item_hovered;
				//UC_LOG_DEBUG(_logger) << "Mouse " << event_type << " at " << node;
			}
		}

		String tooltip;
		if (node.get(UIAttribute::Tooltip).try_get_string(tooltip))
		{
			if (is_item_hovered)
				ImGui::SetTooltip("%s", tooltip.c_str());
		}
	}

	UIViewImGui::CachedInfo* UIViewImGui::get_info(UINodeIndex index)
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
			size = ImGuiConvert::convert(rect.size);
			uv0 = ImGuiConvert::convert(rect.pos / tex_size);
			uv1 = { uv0.x + rect.size.x / tex_size.x, uv0.y + rect.size.y / tex_size.y };
			return true;
		}

		return false;
	}
}
#endif