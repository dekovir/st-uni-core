#pragma once
#include "example.hpp"
#include "unicore/ui/UIComponent.hpp"
#include "unicore/ui/UIViewImGui.hpp"
#include "unicore/ui/UITemplate.hpp"

namespace unicore
{
	class Example08 : public Example
	{
	public:
		explicit Example08(const ExampleContext& context);

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		ImGuiContext& _context;
		Shared<UIDocument> _document;
		Shared<UIViewImGui> _view;

		Shared<ui::Template> _root;
	};
}