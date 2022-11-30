#pragma once
#include "example.hpp"
#include "unicore/remoteui/Element.hpp"
#include "unicore/remoteui/ViewImGui.hpp"

namespace unicore
{
	class Example07 : public Example
	{
	public:
		explicit Example07(const ExampleContext& context);

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		ImGuiContext& _context;
		Shared<remoteui::Document> _document;
		Shared<remoteui::ViewImGui> _view;

		remoteui::Element _position_node;
		remoteui::Element _move_group_node;
		remoteui::Element _move_index_node;

		void update_move_index();
	};
}