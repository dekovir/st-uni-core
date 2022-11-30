#pragma once
#include "unicore/remoteui/View.hpp"
#if defined(UNICORE_USE_IMGUI)
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore::remoteui
{
	class ViewImGui : public View
	{
	public:
		enum class Flag
		{
			NoTiltebar = 1 << 0,
			NoMove = 1 << 1,
			NoResize = 1 << 2,
			NoCollapse = 1 << 3,
			NoBackground = 1 << 4,
		};
		UNICORE_ENUM_FLAGS(Flag, Flags);

		ViewImGui(ImGuiContext& context, Logger& logger);

		UC_NODISCARD const String32& title() const { return _title; }
		void set_title(StringView32 title) { _title = title; }

		UC_NODISCARD const Vector2f& position() const { return _pos; }
		void set_position(const Vector2f& pos) { _set_pos = pos; }

		UC_NODISCARD const Vector2f& size() const { return _size; }
		void set_size(const Vector2f& size) { _set_size = size; }

		Flags& flags() { return _flags; }
		UC_NODISCARD const Flags& flags() const { return _flags; }

		void render();

		UC_NODISCARD bool is_mouse_over() const;

	protected:
		enum class LayoutOption
		{
			None,
			SameLine,
			SameLineFlex,
		};

		Logger& _logger;
		ImGuiContext& _context;
		List<UIEvent> _update_events;

		String32 _title = U"Title";
		Optional<Vector2f> _set_pos, _set_size;

		Vector2f _pos, _size;
		Flags _flags = Flag::NoCollapse;
		Bool _hovered = false;

		struct CachedInfo
		{
			String id;
			String title; // text + id;
			bool mouse_over = false;
		};

		Dictionary<ElementIndex, CachedInfo> _cached;

		void on_rebuild() override;

		void on_create_node(const Element& node) override;
		void on_set_attribute(const Element& node,
			Attribute type, const Optional<Variant>& value) override;

		Bool render_node(const Element& node, LayoutOption layout_option = LayoutOption::None);

		Bool render_group(const CachedInfo& info, const Element& node,
			LayoutOption layout_option = LayoutOption::None);

		Bool render_visual(const CachedInfo& info, const Element& node,
			LayoutOption layout_option = LayoutOption::None);

		Bool render_input(const CachedInfo& info, const Element& node,
			LayoutOption layout_option = LayoutOption::None);

		void render_node_header(const Element& node, LayoutOption layout_option);
		void render_node_footer(const Element& node);

		UC_NODISCARD CachedInfo* get_info(ElementIndex index);

	private:
		unsigned _id;

		static bool get_texture(const Variant& value,
			ImTextureID& id, ImVec2& size, ImVec2& uv0, ImVec2& uv1);
	};
}
#endif