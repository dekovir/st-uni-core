#include "example07.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example07, "UI");

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
			_view->set_title(U"Test UI");
			_view->set_size(Vector2f(300, 0));
			_view->set_position(Vector2f(size.x / 2 - 150, 10));

			_position_node = _document->find_by_id("position");

			const auto group_node = _document->find_by_id("group");
			const auto add_node = _document->find_by_id("add_item");
			const auto combo_node = _document->find_by_id("combo");

			if (!group_node.empty() && !add_node.empty())
			{
				_document->set_node_action(add_node, UIActionType::OnClick,
					[this, group_node]
					{
						const auto count = group_node.get_children_count();
						const auto text = StringBuilder::format("Item {}", count + 1);

						UINodeOptions options;
						options.attributes[UIAttribute::Text] = text;
						_document->create_node(UINodeTag::Item, options, group_node);
					});
			}

			if (!combo_node.empty())
			{
				for (unsigned i = 0; i < 5; i++)
				{
					const auto str = StringBuilder::format("Item {}", i + 1);
					UINodeOptions options;
					options.attributes[UIAttribute::Text] = str;
					options.attributes[UIAttribute::Value] = (i == 0);

					auto node = _document->create_node(UINodeTag::Item, options, combo_node);
					_document->set_node_action(node, UIActionType::OnClick,
						[this, combo_node, node]
						{
							for (const auto& child : combo_node.get_children())
							{
								if (child.tag() == UINodeTag::Item)
									_document->set_node_attribute(child, UIAttribute::Value, false);
							}

							_document->set_node_attribute(node, UIAttribute::Value, true);
							_document->set_node_attribute(combo_node, UIAttribute::Value, node.text());
						});
				}
				_document->set_node_attribute(combo_node, UIAttribute::Value, "Item 1");
			}
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