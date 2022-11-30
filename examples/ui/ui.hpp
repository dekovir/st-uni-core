#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/imgui/ImGuiContext.hpp"
#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/remoteui/Document.hpp"
#include "unicore/remoteui/ViewImGui.hpp"
#include "InventoryUI.hpp"
#include "ItemDatabaseUI.hpp"

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

		Shared<SpriteList> _sprites;

		Shared<ItemDatabase> _database;
		Shared<remoteui::Document> _database_doc;
		Shared<ItemDatabaseUI> _database_ui;
		Shared<remoteui::ViewImGui> _database_view;

		Shared<Inventory> _inventory;
		Shared<remoteui::Document> _inventory_doc;
		Shared<remoteui::ViewImGui> _inventory_view;
		Shared<InventoryUI> _inventory_ui;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}