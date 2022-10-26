#pragma once
#include "unicore/ui/UIView.hpp"
#if defined(UNICORE_USE_IMGUI)
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
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

		void render();

	protected:
		Logger& _logger;
		ImGuiContext& _context;
		List<UIEvent> _update_events;

		String32 _title = U"Title";
		Optional<Vector2f> _set_pos, _set_size;
		Vector2f _pos, _size;

		struct CachedInfo
		{
			String id;
			Variant value;
		};

		Dictionary<UINodeIndex, CachedInfo> _cached;

		void on_rebuild() override;

		void on_create_node(const UINode& node) override;

		void on_set_attribute(const UINode& node,
			UIAttributeType type, const Optional<Variant>& value) override;

		void on_set_action(const UINode& node,
			UIActionType type, const Optional<UIAction>& value) override;

		Bool render_node(const UINode& node, Bool same_line = false);

		void render_node_header(const UINode& node, Bool same_line = false);
		void render_node_footer(const UINode& node);

		UC_NODISCARD CachedInfo* get_info(UINodeIndex index);

	private:
		unsigned _id;
	};
}
#endif