#include "example10.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"
#include "unicore/ui/UIEditor.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example10, "UI Editor");

	class TestObject : public Object
	{
	public:
		int int_value = 42;
		float float_value = 1.5f;

		void print(Logger& logger) const
		{
			UC_LOG_DEBUG(logger) << "Int: " << int_value;
			UC_LOG_DEBUG(logger) << "Float: " << float_value;
		}
	};

	static ui::ObjectEditorContext s_context;

	Example10::Example10(const ExampleContext& context)
		: Example(context)
		, _context(context.imgui)
	{
	}

	void Example10::load(IResourceCache& resources)
	{
		Example::load(resources);

		const auto size = renderer.screen_size();

		_document = std::make_shared<UIDocument>(&logger);
		_view = std::make_shared<UIViewImGui>(_context, logger);
		_view->set_document(_document);
		_view->set_title(U"Editor UI");
		_view->set_size(Vector2f(300, 0));
		_view->set_position(Vector2f(size.x / 2 - 150, 50));

		_object = ui::EditorObjectBuilder(std::make_shared<TestObject>())
			.add("int_value", &TestObject::int_value)
			.add("float_value", &TestObject::float_value)
			.build();

		Shared<ui::ObjectEditorElement> editor_ref;

		_root = ui::ptr(ui::vlayout(
			ui::text(U"Inspector"),
			ref(ui::ObjectEditorElement(s_context), editor_ref),
			ui::button(U"Print", [&] { std::dynamic_pointer_cast<TestObject>(_object->object())->print(logger); })
		));

		if (editor_ref)
			editor_ref->set_object(_object);

		_root->render()->create(*_document, UINode::Empty);
	}

	void Example10::update()
	{
		if (_view)
			_view->render();
	}

	void Example10::draw() const
	{
	}
}