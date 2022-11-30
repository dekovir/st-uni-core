#include "example09.hpp"

#include <utility>
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example09, "UI Elements");

	static const Dictionary<int, String32> items = {
			{0, U"Item 1"},
			{1, U"Item 2"},
			{2, U"Item 3"},
			{3, U"Item 4"},
			{4, U"Item 5"},
	};

	class TestListModel : public ui::ListModel
	{
	public:
		explicit TestListModel(Size count) : _count(count) {}

		UC_NODISCARD Size get_system_memory_use() const override { return sizeof(TestListModel); }
		UC_NODISCARD IndexArg size() const override { return _count; }
		UC_NODISCARD DataType get_at(IndexArg index) const override
		{
			const auto text = StringBuilder::format(U"Item {}", index + 1);
			auto item = ui::ptr(ui::item(text));

			if (index % 5 == 0)
			{
				return ui::ptr(ui::vlayout(
					ui::separator(),
					item
				));
			}

			return item;
		}

	protected:
		Size _count;
	};

	namespace test
	{
		struct Element;

		static constexpr StringView key_children = "children";

		using ChildrenType = Element;
		using ChildrenList = List<ChildrenType>;

		using PropsValue = StdVariant<Variant, ChildrenType, ChildrenList>;
		using Props = Dictionary<StringView, PropsValue>;

		using NodeRender = Function<UINode, const Props&, UIDocument&, const UINode&>;
		using ElementRender = Function<Element, const Props&>;

		using RenderFunc = StdVariant<NodeRender, ElementRender>;

		struct Element
		{
			Props props;
			RenderFunc render;
		};

		using ElementType = StdVariant<ChildrenType, ChildrenList>;

		extern Element createElement(const RenderFunc& render, const Props& props,
			const std::initializer_list<ChildrenType> children = {})
		{
			Element element;
			element.props = props;
			element.render = render;

			if (children.size() > 0)
				element.props[key_children] = children;

			return element;
		}

		template<UIGroupType Type>
		struct node_group_render
		{
			auto operator()(const Props& props, UIDocument& doc, const UINode& parent) const
			{
				UINodeOptions options;
				options.attributes[UIAttribute::Type] = Type;
				return doc.create_node(UINodeTag::Group, {}, parent);
			}
		};

		enum class Tag
		{
			VBox,
			HBox,
		};

		static const Dictionary<Tag, RenderFunc> s_tag_factory =
		{
			{Tag::VBox, node_group_render<UIGroupType::Vertical>()},
			{Tag::HBox, node_group_render<UIGroupType::Horizontal>()},
		};

		extern Element createElement(Tag tag, const Props& props,
			const std::initializer_list<ChildrenType> children = {})
		{
			const auto it = s_tag_factory.find(tag);
			if (it == s_tag_factory.end())
				return {};

			return createElement(it->second, props, children);
		}

		class Document
		{
		public:
			Document(UIDocument& document, Element element, Logger* logger = nullptr)
				: _document(document), _element({ std::move(element) }), _logger(logger)
			{
				render_element(_element);
			}

		protected:
			struct ElementInfo
			{
				Element element;

				UINode parent = UINode::Empty;
				UINode node = UINode::Empty;
			};

			UIDocument& _document;
			ElementInfo _element;
			Logger* _logger;

			void render_element(ElementInfo& info)
			{
				if (const auto render = std::get_if<NodeRender>(&info.element.render))
				{
					info.node = (*render)(info.element.props, _document, info.parent);
					return;
				}

				if (const auto render = std::get_if<ElementRender>(&info.element.render))
				{
					const auto child = (*render)(info.element.props);
					ElementInfo child_info;
					child_info.element = child;
					child_info.parent = info.node;

					render_element(child_info);
					return;
				}
			}
		};
	}

	class TestTableModel : public ui::TableModel
	{
	public:
		explicit TestTableModel(Size size)
			: _size(size)
		{
		}

		UC_NODISCARD Size get_system_memory_use() const override
		{
			return sizeof(TestTableModel);
		}

		UC_NODISCARD IndexArg size() const override { return _size; }

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

		UC_NODISCARD std::shared_ptr<ui::Element> get_at(IndexArg index) const override
		{
			auto str = StringBuilder::format(U"Cell {} {}", index.x + 1, index.y + 1);
			if (Math::even(index.x + index.y))
				return std::make_shared<ui::text>(str);
			return std::make_shared<ui::item>(str);
		}

	protected:
		const IndexType _size;
	};

	Example09::Example09(const ExampleContext& context)
		: Example(context)
		, _context(context.imgui)
	{
	}

	void Example09::load(IResourceCache& resources)
	{
		Example::load(resources);

		const auto size = renderer.screen_size().cast<Float>();

		_document = std::make_shared<UIDocument>(&logger);
		_view = std::make_shared<UIViewImGui>(_context, logger);
		_view->set_document(_document);
		_view->set_title(U"Elements UI");
		_view->set_size(Vector2f(300, 0));
		_view->set_position({ size.x / 2 - 150, 50 });
		_view->set_size({ 300, size.y - 100 });

		Shared<ui::button> btn_ref;
		Shared<ui::combo_box_t<int>> combo_ref;
		Shared<ui::list_box> list_ref;
		Shared<ui::button> add_ref;

		const auto combo_model = std::make_shared<ConstDictionaryDataModel<Int, String32>>(items);
		const auto radio_model = std::make_shared<ConstDictionaryDataModel<Int, String32>>(items);
		const auto list_model = std::make_shared<TestListModel>(10);
		const auto table_model = std::make_shared<TestTableModel>(3);

		_root = ui::ptr(ui::vlayout(
			ui::hlayout(
				ui::text(U"Text"),
				ui::input_text()
			),
			ui::button(U"Click")
		));

		/*_root = ui::ptr(ui::vlayout(
			ui::ref(ui::text(U"<update>"), _position_node),
			ui::hlayout(
				ui::text(U"Text"),
				ui::input_text(U"Lorem ipsum dolor")
			),
			ui::hlayout(
				ui::text(U"Bool"),
				ui::toggle(true),
				ui::radio_button()
			),
			ui::hlayout(
				ui::text(U"Button"),
				ref(ui::button(U"Label"), btn_ref)
			),
			ui::hlayout(
				ui::text(U"Int16"),
				std::make_shared<ui::input_int>(0, [this](auto value) { UC_LOG_DEBUG(logger) << "Changed: " << value; })
			),
			ui::hlayout(
				ui::text(U"UInt8"),
				ui::input_int_t<UInt8>()
			),
			ui::hlayout(
				ui::text(U"Float"),
				ui::input_float(0, 0.1f)
			),
			ui::hlayout(
				ui::text(U"IntRange"),
				ui::range_int({ 0, 100 }, 50)
			),
			ui::hlayout(
				ui::text(U"FloatRange"),
				ui::range_float({ 0, 1 }, 0.5f)
			),
			ui::hlayout(
				ui::text(U"Items"),
				ui::vlayout(
					ui::item(U"Item 1"),
					ui::item(U"Item 2"),
					ui::item(U"Item 3")
				)
			),
			//ui::hlayout(
			//	ui::text(U"Combo"),
			//	ui::ref(ui::combo_box<Int>(combo_model, 0), combo_ref)
			//),
			ui::hlayout(
				ui::text(U"Radio"),
				ui::radio_group_t<Int>(radio_model, 0)
			),
			ui::vlayout(
				ui::text(U"Items"),
				ref(ui::list_box(), list_ref),
				ref(ui::button(U"Add"), add_ref)
			),
			ui::vlayout(
				ui::text(U"Items 2"),
				ui::list(list_model)
			),
			ui::vlayout(
				ui::text(U"Table"),
				ui::table(table_model)
			)
		));/**/

		if (btn_ref)
		{
			btn_ref->on_clicked() += [btn_ref]
			{ btn_ref->set_text(U"Clicked"); };
		}

		//if (combo_ref)
		//{
		//	combo_ref->on_changed() += [this](auto value)
		//	{ UC_LOG_DEBUG(logger) << "Combo value changed to " << value; };
		//}

		if (list_ref && add_ref)
		{
			add_ref->on_clicked() += [this, list_ref]
			{
				const auto text = StringBuilder::format(U"Item {}", list_ref->size() + 1);
				const auto element = ui::hlayout(ui::bullet(), ui::item(text));
				list_ref->add(element);
			};
		}

		_root->mount(*_document, UINode::Empty);
	}

	void Example09::update()
	{
		if (_root)
			_root->update();

		//if (_document && _view && _position_node)
		//{
		//	const auto text = StringBuilder::format(U"Pos {}, Size {}",
		//		_view->position().cast<Int>(), _view->size().cast<Int>());
		//	_position_node->set_text(text);
		//}

		if (_view)
			_view->render();
	}

	void Example09::draw() const
	{
	}
}