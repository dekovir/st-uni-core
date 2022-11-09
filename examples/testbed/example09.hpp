#pragma once
#include "example.hpp"
#include "unicore/ui/UIViewImGui.hpp"

namespace unicore
{
	class Example09 : public Example
	{
	public:
		explicit Example09(const ExampleContext& context);

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		ImGuiContext& _context;
		Shared<UIDocument> _document;
		Shared<UIViewImGui> _view;
	};
}