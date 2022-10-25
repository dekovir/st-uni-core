#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/imgui/ImGuiContext.hpp"
#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/ui/UIDocument.hpp"
#include "unicore/ui/UIViewImGui.hpp"
#include "InventoryUI.hpp"

namespace unicore
{
	class MyApp : public SDLApplication
	{
	public:
		explicit MyApp(const CoreSettings& settings);

	protected:
		ProxyLogger _context_logger;
		ImGuiRender2D _contex_render;
		ImGuiContext _context;

		Shared<SpriteList> _item_sprites;

		Shared<Inventory> _inventory;
		Shared<UIDocument> _document;
		Shared<UIViewImGui> _view;
		Shared<InventoryUI> _inventory_ui;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}