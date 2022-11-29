#include "example07.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example07, "UI Native");

	template<typename T>
	static Bool init_combo_logic(UIDocument& document, const UINode& combo_node,
		const Dictionary<T, StringView>& items, const T start_value,
		const Action<T>& callback = nullptr)
	{
		if (combo_node.empty()) return false;

		for (const auto& it : items)
		{
			const auto value = it.first;

			UINodeOptions options;
			options.attributes[UIAttribute::Text] = it.second;
			options.attributes[UIAttribute::Value] = value == start_value;

			auto node = document.create_input(UIInputType::Item, options, combo_node);
			document.subscribe_node(node, UIActionType::OnClick,
				[&document, combo_node, node, value, callback]
				{
					for (const auto& child : combo_node.get_children())
					{
						if (child.tag() == UINodeTag::Input && 
							child.type().get_enum<UIInputType>() == UIInputType::Item)
							document.set_node_attribute(child, UIAttribute::Value, false);
					}

					document.set_node_attribute(node, UIAttribute::Value, true);
					document.set_node_attribute(combo_node, UIAttribute::Value, node.text());

					if (callback != nullptr)
						callback(value);
				});
		}

		if (const auto it = items.find(start_value); it != items.end())
			document.set_node_attribute(combo_node, UIAttribute::Value, it->second);

		return true;
	}

	static const Dictionary<int, StringView> s_items = {
		{0, "Item 1"},
		{1, "Item 2"},
		{2, "Item 3"},
		{3, "Item 4"},
		{4, "Item 5"},
	};

	Example07::Example07(const ExampleContext& context)
		: Example(context)
		, _context(context.imgui)
	{
	}

	void Example07::load(IResourceCache& resources)
	{
		Example::load(resources);

		const auto size = renderer.screen_size();

		_document = resources.load<UIDocument>("ui.xml"_path, LoggerOption{ logger });
		if (_document)
		{
			_view = std::make_shared<UIViewImGui>(_context, logger);
			_view->set_document(_document);
			_view->set_title(U"Native UI");
			_view->set_size(Vector2f(300, 0));
			_view->set_position(Vector2f(size.x / 2 - 150, 10));

			_position_node = _document->find_by_name("position");

			const auto button_node = _document->find_by_name("button");
			const auto group_node = _document->find_by_name("group");
			const auto add_node = _document->find_by_name("add_item");
			const auto combo_node = _document->find_by_name("combo");

			if (!button_node.empty())
			{
				_document->subscribe_node(button_node, UIActionType::OnClick, [&, button_node]
				{
					_document->set_node_attribute(button_node, UIAttribute::Text, U"Clicked");
				});
			}

			if (!group_node.empty() && !add_node.empty())
			{
				_document->subscribe_node(add_node, UIActionType::OnClick,
					[this, group_node]
					{
						const auto count = group_node.get_children_count();
						const auto text = StringBuilder::format("Item {}", count + 1);

						UINodeOptions options;
						options.attributes[UIAttribute::Text] = text;
						_document->create_input(UIInputType::Item, options, group_node);
					});
			}

			init_combo_logic<int>(*_document, combo_node, s_items, 0,
				[this](int value)
				{
					if (const auto it = s_items.find(value); it != s_items.end())
						UC_LOG_DEBUG(logger) << "Select " << it->second;
					else UC_LOG_DEBUG(logger) << "Select " << value;
				});
		}
	}

	void Example07::update()
	{
		if (_document && _view && !_position_node.empty())
		{
			auto text = StringBuilder::format("Pos {}, Size {}",
				_view->position().cast<Int>(), _view->size().cast<Int>());
			_document->set_node_attribute(_position_node, UIAttribute::Text, text);
		}

		if (_view)
			_view->render();
	}

	void Example07::draw() const
	{
	}
}