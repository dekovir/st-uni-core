#pragma once
#include "example.hpp"
#include "unicore/ui/UINode.hpp"
#include "unicore/ui/UIViewImGui.hpp"

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
		Shared<UIDocument> _document;
		Shared<UIViewImGui> _view;
		UINode _position_node;
	};
}