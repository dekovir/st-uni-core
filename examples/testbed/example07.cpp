#include "example07.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example07, "Remote UI");

	template<typename T>
	static Bool init_combo_logic(remoteui::Document& document,
		const remoteui::Element& combo_node,
		const Dictionary<T, StringView>& items, const T start_value,
		const Action<T>& callback = nullptr)
	{
		if (combo_node.empty()) return false;

		for (const auto& it : items)
		{
			const auto value = it.first;

			remoteui::ElementOptions options;
			options.attributes[remoteui::Attribute::Text] = it.second;
			options.attributes[remoteui::Attribute::Value] = value == start_value;

			auto node = document.create_input(remoteui::InputType::Item, options, combo_node);
			document.subscribe_node(node, remoteui::UIActionType::OnClick,
				[&document, combo_node, node, value, callback]
				{
					for (const auto& child : combo_node.get_children())
					{
						if (child.tag() == remoteui::ElementTag::Input &&
							child.type().get_enum<remoteui::InputType>() == remoteui::InputType::Item)
							document.set_node_attribute(child, remoteui::Attribute::Value, false);
					}

				document.set_node_attribute(node, remoteui::Attribute::Value, true);
				document.set_node_attribute(combo_node, remoteui::Attribute::Value, node.text());

				if (callback != nullptr)
					callback(value);
				});
		}

		if (const auto it = items.find(start_value); it != items.end())
			document.set_node_attribute(combo_node, remoteui::Attribute::Value, it->second);

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

		_document = resources.load<remoteui::Document>("ui.xml"_path, LoggerOption{ logger });
		if (_document)
		{
			_view = std::make_shared<remoteui::ViewImGui>(_context, logger);
			_view->set_document(_document);
			_view->set_title(U"Remote UI");
			_view->set_size(Vector2f(300, 0));
			_view->set_position(Vector2f(size.x / 2 - 150, 10));

			_position_node = _document->find_by_name("position");

			const auto button_node = _document->find_by_name("button");
			const auto group_node = _document->find_by_name("group");
			const auto add_node = _document->find_by_name("add_item");
			const auto combo_node = _document->find_by_name("combo");

			_move_group_node = _document->find_by_name("move_group");
			_move_index_node = _document->find_by_name("move_index");

			const auto move_up_node = _document->find_by_name("move_up");
			const auto move_down_node = _document->find_by_name("move_down");

			if (!button_node.empty())
			{
				_document->subscribe_node(button_node, remoteui::UIActionType::OnClick, [&, button_node]
					{
						_document->set_node_attribute(button_node, remoteui::Attribute::Text, U"Clicked");
					});
			}

			if (!group_node.empty() && !add_node.empty())
			{
				_document->subscribe_node(add_node, remoteui::UIActionType::OnClick,
					[this, group_node]
					{
						const auto count = group_node.get_children_count();
					const auto text = StringBuilder::format("Item {}", count + 1);

					remoteui::ElementOptions options;
					options.attributes[remoteui::Attribute::Text] = text;
					_document->create_input(remoteui::InputType::Item, options, group_node);
					});
			}

			update_move_index();
			if (!_move_group_node.empty() && !_move_index_node.empty() &&
				!move_up_node.empty() && !move_down_node.empty())
			{
				_document->subscribe_node(move_up_node, remoteui::UIActionType::OnClick, [&]
					{
						const auto index = _document->get_node_sibling_index(_move_group_node);
				_document->set_node_sibling_index(_move_group_node, index - 1);
				update_move_index();
					});

				_document->subscribe_node(move_down_node, remoteui::UIActionType::OnClick, [&]
					{
						const auto index = _document->get_node_sibling_index(_move_group_node);
				_document->set_node_sibling_index(_move_group_node, index + 1);
				update_move_index();
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
			_document->set_node_attribute(_position_node, remoteui::Attribute::Text, text);
		}

		if (_view)
			_view->render();
	}

	void Example07::draw() const
	{
	}

	void Example07::update_move_index()
	{
		if (_move_group_node.empty() || _move_index_node.empty()) return;

		const auto index = _document->get_node_sibling_index(_move_group_node);
		const auto text = StringBuilder::format("{}", index);
		_document->set_node_attribute(_move_index_node, remoteui::Attribute::Text, text);
	}
}