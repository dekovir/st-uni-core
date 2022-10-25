#include "ui.hpp"
#include "UnicoreMain.hpp"
#include "InitPlugins.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/platform/Time.hpp"
#include "unicore/platform/Input.hpp"
#include "unicore/xml/XMLData.hpp"

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

	static const auto xml_text = R"(
	<group>
		<text>Sample text</text>
		<group value="2" tooltip="Tooltip text">
			<text>Input</text>
			<input value="quick brown fox" />
		</group>
		<group value="2">
			<text>Slider</text>
			<slider id="slider" value="50.50" max="100">Value %.1f</slider>
		</group>
		<group value="2">
			<text>Toggle</text>
			<toggle value="true" />
		</group>
		<group id="group" />
		<button id="add_item">Add item</button>
	</group>
	)";

	static const Dictionary<StringView, UINodeType> s_tag_type =
	{
		{"window", UINodeType::Window},
		{"group", UINodeType::Group},
		{"text", UINodeType::Text},
		{"button", UINodeType::Button},
		{"input", UINodeType::Input},
		{"slider", UINodeType::Slider},
		{"toggle", UINodeType::Toggle},
		{"tooltip", UINodeType::Tooltip},
		{"item", UINodeType::Item},
	};

	static const Dictionary<StringView, UIAttributeType> s_attr_name =
	{
		{"value", UIAttributeType::Value},
		{"tooltip", UIAttributeType::Tooltip},
		{"min", UIAttributeType::MinValue},
		{"max", UIAttributeType::MaxValue},
	};

	static Variant parse_value(const char* str)
	{
		char* end;

		const auto i = strtoll(str, &end, 10);
		if (end[0] == 0)
			return i;

		const auto d = strtod(str, &end);
		if (end[0] == 0)
			return d;

		return str;
	}

	static UINodeType parse_tag(StringView tag)
	{
		for (const auto& it : s_tag_type)
		{
			if (StringHelper::equals(it.first, tag, true))
				return it.second;
		}

		return UINodeType::None;
	}

	static void parse_node_recurse(const tinyxml2::XMLElement* node, UIDocument& doc, UINodeIndex parent)
	{
		const auto tag = StringView(node->Value());
		const auto node_type = parse_tag(tag);

		UINodeOptions options;

		// Fill attributes
		if (const auto value = node->GetText(); value != nullptr)
			options.attributes[UIAttributeType::Text] = parse_value(value);

		if (const auto str = node->Attribute("id"); str != nullptr)
			options.uid = str;

		if (const auto str = node->Attribute("name"); str != nullptr)
			options.name = str;

		for (const auto& [name, type] : s_attr_name)
		{
			if (const auto t = node->Attribute(name.data()); t != nullptr)
				options.attributes[type] = parse_value(t);
		}

		const auto index = doc.create_node(node_type, parent, options);
		for (auto child = node->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
			parse_node_recurse(child, doc, index);
	}

	static void parse_xml(const XMLData& data, UIDocument& doc)
	{
		const auto root = data.doc.RootElement();
		if (!root) return;

		parse_node_recurse(root, doc, UINodeIndexInvalid);
	}

	void MyApp::on_init()
	{
		UC_LOG_INFO(logger) << "Starting";

		_contex_render.init(renderer);

		_inventory = std::make_shared<Inventory>(16);

		_document = std::make_shared<UIDocument>(&_context_logger);
		_view = std::make_shared<UIViewImGui>(_context, _context_logger);
		_view->set_document(_document);

#if 1
		XMLData xml;
		xml.doc.Parse(xml_text);
		parse_xml(xml, *_document);

		const auto slider_id = _document->find_index_by_id("slider");
		const auto group_id = _document->find_index_by_id("group");
		const auto add_id = _document->find_index_by_id("add_item");

		if (group_id != UINodeIndexInvalid && add_id != UINodeIndexInvalid)
		{
			_document->set_node_action(add_id, UIActionType::OnClick,
				[this, group_id]()
				{
					static int index = 1;
					const auto text = StringBuilder::format("Item {}", index++);

					UINodeOptions options;
					options.attributes[UIAttributeType::Text] = text;
					_document->create_node(UINodeType::Item, group_id, options);
				});
		}

		if (slider_id != UINodeIndexInvalid)
		{
			_document->set_node_action(slider_id, UIActionType::OnChange,
				[this](const Variant& value)
				{
					UC_LOG_DEBUG(logger) << "Slider value changed to " << value;
				});
		}
#else
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
			}
}
});
#endif
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

		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		_view->render();

		_context.frame_end();
	}

	void MyApp::on_draw()
	{
		renderer.clear(ColorConst4b::Black);

		_context.render();
	}

	UNICORE_MAIN_CORE(MyApp);
	}