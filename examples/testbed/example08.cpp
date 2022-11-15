#include "example08.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example08, "UI Components");

	static const Dictionary<int, String32> items = {
			{0, U"Item 1"},
			{1, U"Item 2"},
			{2, U"Item 3"},
			{3, U"Item 4"},
			{4, U"Item 5"},
	};

	class TestTableModel : public ui::TableDataModel<Shared<ui::Component>>
	{
	public:
		explicit TestTableModel(Size size)
			: _size(size)
		{
		}

		UC_NODISCARD Size size() const override { return _size; }
		UC_NODISCARD Size col_count() const override { return 3; }

		UC_NODISCARD StringView32 get_header(Size col) const override
		{
			switch (col)
			{
			case 0: return U"Col 1";
			case 1: return U"Col 2";
			case 2: return U"Col 3";
			default: return U"Err";
			}
		}

		UC_NODISCARD std::shared_ptr<ui::Component> get_at(Size row, Size column) const override
		{
			auto str = StringBuilder::format(U"Cell {} {}", row + 1, column + 1);
			if (Math::even(row + column))
				return std::make_shared<ui::Text>(str);
			return std::make_shared<ui::Item>(str);
		}

	protected:
		const Size _size;
	};

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
		_view->set_title(U"Test UI");
		_view->set_size(Vector2f(300, 0));
		_view->set_position(Vector2f(size.x / 2 - 150, 50));

		Shared<ui::Button> btn_ref;
		Shared<ui::ComboBox<int>> combo_ref;
		Shared<ui::list_box> list_ref;
		Shared<ui::Button> add_ref;

		const auto items_model = std::make_shared<ui::ConstDictionaryDataModel<int, String32>>(items);
		const auto table_model = std::make_shared<TestTableModel>(3);

		_root = ui::ptr(ui::vlayout(
			ui::ref(ui::Text(U""), _position_node),
			ui::hlayout(
				ui::Text(U"Text"),
				ui::TextInput(U"Lorem ipsum dolor")
			),
			ui::hlayout(
				ui::Text(U"Bool"),
				ui::Toggle(true),
				ui::RadioButton()
			),
			ui::hlayout(
				ui::Text(U"Button"),
				ref(ui::Button(U"Label"), btn_ref)
			),
			ui::hlayout(
				ui::Text(U"Int16"),
				ui::IntInput()
			),
			ui::hlayout(
				ui::Text(U"UInt8"),
				ui::IntegralInput<UInt8>()
			),
			ui::hlayout(
				ui::Text(U"Float"),
				ui::FloatInput(0, 0.1f)
			),
			ui::hlayout(
				ui::Text(U"IntRange"),
				ui::IntRange({ 0, 100 }, 50)
			),
			ui::hlayout(
				ui::Text(U"FloatRange"),
				ui::FloatRange({ 0, 1 }, 0.5f)
			),
			ui::hlayout(
				ui::Text(U"Items"),
				ui::vlayout(
					ui::Item(U"Item 1"),
					ui::Item(U"Item 2"),
					ui::Item(U"Item 3")
				)
			),
			ui::hlayout(
				ui::Text(U"Combo"),
				ui::ref(ui::ComboBox<int>(items_model, 0), combo_ref)
			),
			ui::vlayout(
				ui::Text(U"Items"),
				ref(ui::list_box(), list_ref),
				ref(ui::Button(U"Add"), add_ref)
			),
			ui::vlayout(
				ui::Text(U"Table"),
				ui::Table(table_model)
			)
		));

		if (btn_ref)
			btn_ref->set_click_action([btn_ref] { btn_ref->set_text(U"Clicked"); });

		if (combo_ref)
			combo_ref->on_changed() +=
			[this](auto value) { UC_LOG_DEBUG(logger) << "Combo value changed to " << value; };

		if (list_ref && add_ref)
		{
			add_ref->set_click_action([this, list_ref]
				{
					const auto text = StringBuilder::format(U"Item {}", list_ref->size() + 1);
			list_ref->add(ui::Item(text));
				});
		}

		_root->mount(*_document, UINode::Empty);
	}

	void Example08::update()
	{
		if (_document && _view && _position_node)
		{
			const auto text = StringBuilder::format(U"Pos {}, Size {}",
				_view->position().cast<Int>(), _view->size().cast<Int>());
			_position_node->set_text(text);
		}

		if (_view)
			_view->render();
	}

	void Example08::draw() const
	{
	}
}