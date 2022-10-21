#pragma once
#include "unicore/ui/UIView.hpp"
#include "unicore/imgui/ImGuiContext.hpp"
#include "Inventory.hpp"

namespace unicore
{
	class UIViewImGui : public UIView
	{
	public:
		UIViewImGui(ImGuiContext& context, Logger& logger);

		void render();

	protected:
		Logger& _logger;
		ImGuiContext& _context;
		List<UIEvent> _update_events;

		struct CachedInfo
		{
			String id;
		};

		Dictionary<UINodeIndex, CachedInfo> _cached;

		void on_create_node(const UINode& node) override;

		void on_set_attribute(const UINode& node,
			UIAttributeType type, const Optional<UIAttributeValue>& value) override;

		void on_set_action(const UINode& node,
			UIActionType type, const Optional<UIAction>& value) override;

		void render_node(const UINode& node);

		UC_NODISCARD CachedInfo* get_info(UINodeIndex index);
	};
}