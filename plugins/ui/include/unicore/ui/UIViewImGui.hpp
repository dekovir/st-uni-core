#pragma once
#include "unicore/ui/UIView.hpp"
#if defined(UNICORE_USE_IMGUI)
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	enum class UIViewImGuiFlag
	{
		NoTiltebar = 1 << 0,
		NoMove = 1 << 1,
		NoResize = 1 << 2,
		NoCollapse = 1 << 3,
		NoBackground = 1 << 4,
	};
	UNICORE_ENUM_FLAGS(UIViewImGuiFlag, UIViewImGuiFlags);

	class UIViewImGui : public UIView
	{
	public:
		UIViewImGui(ImGuiContext& context, Logger& logger);

		UC_NODISCARD const String32& title() const { return _title; }
		void set_title(StringView32 title) { _title = title; }

		UC_NODISCARD const Vector2f& position() const { return _pos; }
		void set_position(const Vector2f& pos) { _set_pos = pos; }

		UC_NODISCARD const Vector2f& size() const { return _size; }
		void set_size(const Vector2f& size) { _set_size = size; }

		UIViewImGuiFlags& flags() { return _flags; }
		UC_NODISCARD const UIViewImGuiFlags& flags() const { return _flags; }

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
		UIViewImGuiFlags _flags = UIViewImGuiFlag::NoCollapse;
		Bool _hovered = false;

		struct CachedInfo
		{
			String id;
			String title; // text + id;
			bool mouse_over = false;
		};

		Dictionary<UINode::IndexType, CachedInfo> _cached;

		void on_rebuild() override;

		void on_create_node(const UINode& node) override;
		void on_set_attribute(const UINode& node,
			UIAttributeType type, const Optional<Variant>& value) override;

		Bool render_node(const UINode& node, LayoutOption layout_option = LayoutOption::None);

		void render_node_header(const UINode& node, LayoutOption layout_option);
		void render_node_footer(const UINode& node);

		UC_NODISCARD CachedInfo* get_info(UINode::IndexType index);

	private:
		unsigned _id;

		static bool get_texture(const Variant& value,
			ImTextureID& id, ImVec2& size, ImVec2& uv0, ImVec2& uv1);
	};
}
#endif