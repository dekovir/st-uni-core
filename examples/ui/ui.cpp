#include "ui.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/platform/Input.hpp"

namespace unicore
{
	constexpr Vector2i WindowSize = Vector2i(1024, 768);
	constexpr DisplayWindowFlags WindowFlags = DisplayWindowFlag::Resizable;

	MyApp::MyApp(const CoreSettings& settings)
		: SDLApplication(create_settings(settings, "UI", { false, WindowSize, WindowFlags }))
		, _context_logger("[UI] ", logger)
		, _contex_render(renderer, _context_logger)
		, _context(_contex_render, time, input, _context_logger)
	{
		init_plugins(*this);
	}

	void MyApp::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";

		_contex_render.init(renderer);

		_inventory = std::make_shared<Inventory>(16);

		_document = std::make_shared<UIDocument>(&_context_logger);
		_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_view->set_document(_document);

		_document->create_node(UINodeType::Text, UINodeIndexInvalid,
			{ {UIAttributeType::Value, "Sample text"} });

		_document->create_node(UINodeType::Input, UINodeIndexInvalid,
			{
				{UIAttributeType::Value, "quick brown fox"}
			});

		_document->create_node(UINodeType::Slider, UINodeIndexInvalid,
			{
				{UIAttributeType::MaxValue, 100},
				{UIAttributeType::Value, 50}
			});

		const auto group = _document->create_node(UINodeType::Group, UINodeIndexInvalid);

		_document->create_node(UINodeType::Button, UINodeIndexInvalid,
			{ {UIAttributeType::Value, "Button"} },
			{ {UIActionType::OnClick, [this, group]()
			{
				static int index = 1;
				auto text = StringBuilder::format("Item {}", index++);
				_document->create_node(UINodeType::Text, group,
					{ {UIAttributeType::Value, text} });
			}} });
	}

	void MyApp::on_update()
	{
#if !defined(UNICORE_PLATFORM_WEB)
		if (input.keyboard().down(KeyCode::Escape))
			platform.looper.quit();

		if (
			input.keyboard().down_changed(KeyCode::Enter) &&
			input.keyboard().mods(KeyModCombine::Alt))
		{
			auto mode = display.get_mode();
			mode.fullscreen = !mode.fullscreen;
			mode.window_size = WindowSize;
			mode.window_flags = WindowFlags;
			display.set_mode(mode);
		}
#endif

		// ImGui //////////////////////////////////////////////////////////////////
		_context.frame_begin();

		//static bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);

		_view->update();

		_context.frame_end();
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_context.render();
	}

	UNICORE_MAIN_CORE(MyApp);
}