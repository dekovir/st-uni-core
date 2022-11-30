#include "unicore/remoteui/ViewImGui.hpp"
#if defined(UNICORE_USE_IMGUI)
#include "unicore/system/StringHelper.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/renderer/Sprite.hpp"
#include <imgui_stdlib.h>

namespace unicore::remoteui
{
	static unsigned s_last_id = 0;

	ViewImGui::ViewImGui(ImGuiContext& context, Logger& logger)
		: _logger(logger)
		, _context(context)
		, _pos(0, 0)
		, _size(0, 0)
		, _id(s_last_id++)
	{
	}

	void ViewImGui::render()
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
		if (_flags.has(Flag::NoTiltebar))
			window_flags |= ImGuiWindowFlags_NoTitleBar;

		if (_flags.has(Flag::NoMove))
			window_flags |= ImGuiWindowFlags_NoMove;

		if (_flags.has(Flag::NoResize))
			window_flags |= ImGuiWindowFlags_NoResize;

		if (_flags.has(Flag::NoCollapse))
			window_flags |= ImGuiWindowFlags_NoCollapse;

		if (_flags.has(Flag::NoBackground))
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

	bool ViewImGui::is_mouse_over() const
	{
		return _hovered;
	}

	void ViewImGui::on_rebuild()
	{
		_cached.clear();

		if (_document)
		{
			for (const auto& node : _document->get_roots())
				on_create_node(node);
		}
	}

	void ViewImGui::on_create_node(const Element& node)
	{
		//UC_LOG_DEBUG(_logger) << "create_node " << node;

		CachedInfo info;
		info.id = StringBuilder::format("##{}", StringHelper::to_hex(node.index().value));
		info.title = node.text().get_string() + info.id;

		_cached[node.index()] = info;

		for (const auto& child : node.get_children())
			on_create_node(child);
	}

	void ViewImGui::on_set_attribute(const Element& node,
		Attribute type, const Optional<Variant>& value)
	{
		View::on_set_attribute(node, type, value);

		if (type == Attribute::Text)
		{
			if (const auto info = get_info(node.index()))
				info->title = value->get_string() + info->id;
		}
	}

	Bool ViewImGui::render_node(const Element& node, LayoutOption layout_option)
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
		case ElementTag::Group: // GROUP ////////////////////////////////////////////
			return render_group(*cached_info, node, layout_option);

		case ElementTag::Visual: // VISUAL //////////////////////////////////////////
			return render_visual(*cached_info, node, layout_option);

		case ElementTag::Input: // INPUT ////////////////////////////////////////////
			return render_input(*cached_info, node, layout_option);
		}

		return false;
	}

	Bool ViewImGui::render_group(const CachedInfo& info,
		const Element& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto& title = info.title;

		const auto width = node.get(Attribute::Width).get_float();
		const auto height = node.get(Attribute::Height).get_float();

		Bool bool_value;
		String str;
		String32 str32;

		List<Element> children;
		node.get_children(children);

		switch (node.type().get_enum<GroupType>())
		{
		case GroupType::Vertical: // VERTICAL ////////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case GroupType::Horizontal: // HORIZONTAL ////////////////////////////////
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

		case GroupType::Child: // CHILD //////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginChild(id.c_str(), { width, height }, false, ImGuiWindowFlags_AlwaysAutoResize))
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::EndChild();
			render_node_footer(node);
			return true;

		case GroupType::List: // LIST ////////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginListBox(id.c_str(), { width, height }))
			{
				for (const auto& child : children)
					render_node(child);
				ImGui::EndListBox();
			}
			render_node_footer(node);
			return true;

		case GroupType::Tree: // TREE ////////////////////////////////////////////
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

		case GroupType::Combo: // COMBO //////////////////////////////////////////
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

		case GroupType::Flex: // FLEX ////////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::BeginListBox(id.c_str(), { width, height }))
			{
				for (const auto& child : children)
					render_node(child, LayoutOption::SameLineFlex);
				ImGui::EndListBox();
			}
			render_node_footer(node);
			return true;

		case GroupType::Table: // TABLE //////////////////////////////////////////
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

		case GroupType::TableHeader: // TABLE HEADER /////////////////////////////
			render_node_header(node, layout_option);
			str = node.text().get_string();
			ImGui::TableNextColumn();
			ImGui::TableHeader(str.c_str());
			render_node_footer(node);
			return true;

		case GroupType::TableRow: // TABLE ROW ///////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::TableNextRow();
			ImGui::BeginGroup();
			for (const auto& child : children)
				render_node(child);
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case GroupType::TableCell: // TABLE CELL /////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::TableNextColumn();
			ImGui::BeginGroup();
			if (node.get(Attribute::Text).try_get_string(str))
				ImGui::Text("%s", str.c_str());

			if (!children.empty())
			{
				for (const auto& child : children)
					render_node(child);
			}
			ImGui::EndGroup();
			render_node_footer(node);
			return true;

		case GroupType::Popup: // POPUP //////////////////////////////////////////
			if (node.value().get_bool())
			{
				ImGui::OpenPopup(id.c_str());
				_document->set_node_attribute(node, Attribute::Value, false);
			}

			if (ImGui::BeginPopup(id.c_str()))
			{
				for (const auto& child : children)
					render_node(child);

				ImGui::EndPopup();
			}
			break;

		case GroupType::Tooltip: // TOOLTIP //////////////////////////////////////
			ImGui::BeginTooltip();

			str = node.get(Attribute::Text).get_string();
			if (!str.empty())
				ImGui::Text("%s", str.c_str());

			for (const auto& child : children)
				render_node(child);
			ImGui::EndTooltip();
			break;

		case GroupType::Modal: // MODAL //////////////////////////////////////////
			if (node.value().get_bool())
			{
				ImGui::OpenPopup(id.c_str());
				_document->set_node_attribute(node, Attribute::Value, false);
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

	Bool ViewImGui::render_visual(const CachedInfo& info,
		const Element& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto width = node.get(Attribute::Width).get_float();
		const auto height = node.get(Attribute::Height).get_float();

		Float float_value;
		String str;
		String32 str32;
		Rangef range_f;
		Color4f col4_value;

		ImTextureID texture_id;
		ImVec2 size, uv0, uv1;

		switch (node.type().get_enum<VisualType>())
		{
		case VisualType::Text: // TEXT ///////////////////////////////////////////
			str = node.get(Attribute::Text).get_string();

			render_node_header(node, layout_option);
			ImGui::Text("%s", str.c_str());
			render_node_footer(node);
			return true;

		case VisualType::Color: // COLOR /////////////////////////////////////////
			col4_value = node.value().get_color4f();

			render_node_header(node, layout_option);
			if (ImGui::ColorButton(id.c_str(), ImGuiConvert::convert_color(col4_value)))
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });

			render_node_footer(node);
			return true;

		case VisualType::Image: // IMAGE /////////////////////////////////////////
			render_node_header(node, layout_option);
			if (get_texture(node.value(), texture_id, size, uv0, uv1))
			{
				const ImVec2 s = { width > 0 ? width : size.x, height > 0 ? height : size.y };
				ImGui::Image(texture_id, s, uv0, uv1);
			}
			else ImGui::Image(nullptr, { width, height });
			render_node_footer(node);
			return true;

		case VisualType::Progress: // PROGRESS ///////////////////////////////////
			range_f = {
				node.get(Attribute::Min).get_float(0),
				node.get(Attribute::Max).get_float(1)
			};
			float_value = Math::inverse_lerp(
				range_f.min, range_f.max, node.value().get_float());
			render_node_header(node, layout_option);
			if (node.get(Attribute::Text).try_get_string(str))
				ImGui::ProgressBar(float_value, { width, height }, str.c_str());
			else ImGui::ProgressBar(float_value, { width, height });
			render_node_footer(node);
			return true;

		case VisualType::Separator: // SEPARATOR /////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::Separator();
			render_node_footer(node);
			return true;

		case VisualType::Bullet: // BULLET ///////////////////////////////////////
			render_node_header(node, layout_option);
			ImGui::Bullet();
			render_node_footer(node);
			return true;
		}

		return false;
	}

	Bool ViewImGui::render_input(const CachedInfo& info,
		const Element& node, LayoutOption layout_option)
	{
		const auto& id = info.id;
		const auto& title = info.title;

		const auto width = node.get(Attribute::Width).get_float();
		const auto height = node.get(Attribute::Height).get_float();

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
		switch (node.type().get_enum<InputType>())
		{
		case InputType::Text: // TEXT ////////////////////////////////////////////
			render_node_header(node, layout_option);
			str = node.value().get_string();
			if (ImGui::InputText(id.c_str(), &str))
				_update_events.push_back({ node, UIActionType::OnChange, str });
			render_node_footer(node);
			break;

		case InputType::TextArea: // TEXTAREA ////////////////////////////////////
			render_node_header(node, layout_option);
			str = node.value().get_string();
			if (ImGui::InputTextMultiline(id.c_str(), &str, { width, height }))
				_update_events.push_back({ node, UIActionType::OnChange, str });
			render_node_footer(node);
			break;

		case InputType::Toggle: // TOGGLE ////////////////////////////////////////
			render_node_header(node, layout_option);
			bool_value = node.value().get_bool();
			if (ImGui::Checkbox(id.c_str(), &bool_value))
				_update_events.push_back({ node, UIActionType::OnChange, bool_value });
			render_node_footer(node);
			break;

		case InputType::Radio: // RADIO //////////////////////////////////////////
			render_node_header(node, layout_option);
			bool_value = node.value().get_bool();
			if (ImGui::RadioButton(id.c_str(), bool_value))
			{
				_update_events.push_back({ node, UIActionType::OnChange, !bool_value });
				ImGui::CloseCurrentPopup();
			}
			render_node_footer(node);
			break;

		case InputType::Button: // BUTTON ////////////////////////////////////////
			render_node_header(node, layout_option);
			if (ImGui::Button(title.c_str(), { width, height }))
			{
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
				ImGui::CloseCurrentPopup();
			}
			render_node_footer(node);
			break;

		case InputType::Item: // ITEM ////////////////////////////////////////////
			bool_value = node.value().get_bool();
			render_node_header(node, layout_option);
			if (ImGui::Selectable(title.c_str(), bool_value))
				_update_events.push_back({ node, UIActionType::OnClick, Variant::Empty });
			render_node_footer(node);
			break;

		case InputType::Image: // IMAGE //////////////////////////////////////////
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

		case InputType::Integer: // INTEGER //////////////////////////////////////
			range_i = {
				node.get(Attribute::Min).get_int(std::numeric_limits<Int>::min()),
				node.get(Attribute::Max).get_int(std::numeric_limits<Int>::max())
			};

			int_value = node.value().get_int();
			if (ImGui::InputInt(id.c_str(), &int_value,
				node.get(Attribute::Step).get_int(0)))
			{
				int_value = range_i.clamp(int_value);
				_update_events.push_back({ node, UIActionType::OnChange, int_value });
			}
			break;

		case InputType::Float: // FLOAT //////////////////////////////////////////
			range_f = {
				node.get(Attribute::Min).get_float(-std::numeric_limits<Float>::max()),
				node.get(Attribute::Max).get_float(+std::numeric_limits<Float>::max())
			};
			float_value = node.value().get_float();
			if (ImGui::InputFloat(id.c_str(), &float_value,
				node.get(Attribute::Step).get_float(0)))
			{
				float_value = range_f.clamp(float_value);
				_update_events.push_back({ node, UIActionType::OnChange, float_value });
			}

			break;

		case InputType::RangeI: // RANGEI ////////////////////////////////////////
			// TODO: Implement step
			range_i = {
				node.get(Attribute::Min).get_int(0),
				node.get(Attribute::Max).get_int(100)
			};
			str = node.get(Attribute::Text).get_string("%d");
			int_value = node.value().get_int();
			if (ImGui::SliderInt(id.c_str(), &int_value,
				range_i.min, range_i.max, str.c_str()))
				_update_events.push_back({ node, UIActionType::OnChange, int_value });
			break;

		case InputType::RangeF: // RANGEF ////////////////////////////////////////
			// TODO: Implement step
			range_f = {
				node.get(Attribute::Min).get_float(0),
				node.get(Attribute::Max).get_float(1)
			};
			str = node.get(Attribute::Text).get_string("%.2f");
			float_value = node.value().get_float();
			if (ImGui::SliderFloat(id.c_str(), &float_value,
				range_f.min, range_f.max, str.c_str()))
				_update_events.push_back({ node, UIActionType::OnChange, float_value });
			break;

		case InputType::Color3: // COLOR3 ////////////////////////////////////////
			col3_value = node.value().get_color3f();
			if (ImGui::ColorEdit3(id.c_str(), &col3_value.r))
				_update_events.push_back({ node, UIActionType::OnChange, col3_value });
			break;

		case InputType::Color4: // COLOR4 ////////////////////////////////////////
			col4_value = node.value().get_color4f();
			if (ImGui::ColorEdit4(id.c_str(), &col4_value.r))
				_update_events.push_back({ node, UIActionType::OnChange, col4_value });
			break;
		}

		render_node_footer(node);
		return true;
	}

	void ViewImGui::render_node_header(
		const Element& node, LayoutOption layout_option)
	{
		if (node.disabled())
			ImGui::BeginDisabled(true);

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

	void ViewImGui::render_node_footer(const Element& node)
	{
		const bool is_item_hovered = ImGui::IsItemHovered();

		if (node.disabled())
			ImGui::EndDisabled();

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
		if (node.get(Attribute::Tooltip).try_get_string(tooltip))
		{
			if (is_item_hovered)
				ImGui::SetTooltip("%s", tooltip.c_str());
		}
	}

	ViewImGui::CachedInfo* ViewImGui::get_info(ElementIndex index)
	{
		const auto it = _cached.find(index);
		return it != _cached.end() ? &it->second : nullptr;
	}

	bool ViewImGui::get_texture(const Variant& value,
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