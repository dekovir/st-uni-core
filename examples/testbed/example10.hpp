#pragma once
#include "example.hpp"
#include "unicore/ui/UIComponent.hpp"
#include "unicore/ui/UIViewImGui.hpp"
#include "unicore/ui/editors/EditorComponent.hpp"

namespace unicore
{
	class Example10 : public Example
	{
	public:
		explicit Example10(const ExampleContext& context);

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		ImGuiContext& _context;
		Shared<UIDocument> _document;
		Shared<UIViewImGui> _view;

		Shared<ui::Component> _root;
		Shared<ui::EditorObject> _object;
	};
}