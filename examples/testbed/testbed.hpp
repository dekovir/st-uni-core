#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/renderer/SpriteBatch.hpp"
#include "unicore/imgui/ImGuiContext.hpp"
#include "unicore/imgui/ImGuiRender.hpp"
#include "unicore/remoteui/Document.hpp"
#include "unicore/remoteui/ViewImGui.hpp"
#include "example.hpp"

namespace unicore
{
	class ReadFileProvider;

	class MyApp : public SDLApplication
	{
	public:
		explicit MyApp(const CoreSettings& settings);

	protected:
		Unique<Example> _example;
		int _example_index = -1;

		Shared<Font> _font;
		Shared<ReadFileProvider> _archive;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;

		uint32_t _draw_calls = 0;

		List<String32> _lines;

		ProxyLogger _ui_logger;

		ImGuiRender2D _ui_render;
		ImGuiContext _ui_context;

		Shared<remoteui::Document> _ui_document;
		Shared<remoteui::ViewImGui> _ui_view;

		void on_init() override;
		void on_update() override;
		void on_draw() override;

		void on_drop_file(const Path& path) override;

		void set_example(int index);
	};
}