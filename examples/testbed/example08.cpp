#include "example08.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example08, "UI Templates");

	Example08::Example08(const ExampleContext& context)
		: Example(context)
		, _context(context.imgui)
	{
	}

	void Example08::load(IResourceCache& resources)
	{
		Example::load(resources);

		const auto size = renderer.screen_size();

		_document = std::make_shared<UIDocument>(&logger);
		_view = std::make_shared<UIViewImGui>(_context, logger);
		_view->set_document(_document);
		_view->set_title(U"Template UI");
		_view->set_size(Vector2f(300, 0));
		_view->set_position(Vector2f(size.x / 2 - 150, 50));

		_root = ui::ptr(ui::GroupV(
			ui::GroupH(
				ui::VisualText({ ui::attr::Text(U"Color3") }),
				ui::InputColor3()
			),
			ui::InputButton({ ui::attr::Text(U"Click") })
		));

		_root->create(*_document, UINode::Empty);
	}

	void Example08::update()
	{
		if (_view)
			_view->render();
	}

	void Example08::draw() const
	{
	}
}