#include "example08.hpp"
#include "unicore/system/Event.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example08, "UI 2");

	namespace ui
	{
		class Component
		{
		public:
			explicit Component(UINodeTag tag)
				: _tag(tag)
			{
			}

			virtual ~Component() = default;

			UC_NODISCARD Bool is_mounted() const { return !_node.empty(); }

			UC_NODISCARD UINodeTag tag() const { return _tag; }
			UC_NODISCARD const UINode& node() const { return _node; }

			UC_NODISCARD UIDocument* document() { return _document; }
			UC_NODISCARD const UIDocument* document() const { return _document; }

			void mount(UIDocument& document, const UINode& parent)
			{
				if (is_mounted()) return;

				_document = &document;
				_node = _document->create_node(_tag, _options, parent);

				on_mount();
			}

			void dismount()
			{
				if (!is_mounted()) return;

				on_dismount();

				_document->remove_node(_node);
				_document = nullptr;
				_node = UINode::Empty;
			}

		protected:
			virtual void on_mount() {}
			virtual void on_dismount() {}

			UC_NODISCARD Variant get_attribute(UIAttribute type) const { return _node.get(type); }

			void set_attribute(UIAttribute type, const Variant& value)
			{
				_options.attributes[type] = value;
				if (_document)
					_document->set_node_attribute(_node, type, value);
			}

			void set_action(UIActionType type, const UIAction& action)
			{
				_options.actions[type] = action;
				if (_document)
					_document->subscribe_node(_node, type, action);
			}

		private:
			const UINodeTag _tag;
			UINodeOptions _options;

			UIDocument* _document = nullptr;
			UINode _node = UINode::Empty;
		};

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		extern Shared<T> ref(T&& element, Shared<T>& value)
		{
			value = std::make_shared<T>(std::forward<T>(element));
			return value;
		}

		class LayoutComponent : public Component
		{
		public:
			explicit LayoutComponent(UIGroupType type)
				: Component(UINodeTag::Group)
			{
				set_attribute(UIAttribute::Type, type);
			}

			UC_NODISCARD Size size() const { return _children.size(); }

			Shared<Component> add(const Shared<Component>& component)
			{
				// TODO: Create/destroy
				_children.push_back(component);
				if (is_mounted())
					component->mount(*document(), node());

				return component;
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
			void add(const T& component)
			{
				auto ptr = std::make_shared<T>(component);
				add(ptr);
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
			void add(T&& component)
			{
				auto ptr = std::make_shared<T>(std::move(component));
				add(ptr);
			}

			template<typename T,
				typename ... Args,
				std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr,
				std::enable_if_t<std::is_constructible_v<T, Args...>>* = nullptr>
			Shared<T> create(Args... args)
			{
				auto component = std::make_shared<T>(std::forward<Args>(args)...);
				add(component);
				return component;
			}

		protected:
			List<Shared<Component>> _children;

			void on_mount() override
			{
				for (const auto& child : _children)
					child->mount(*document(), node());
			}

			void on_dismount() override
			{
				for (const auto& child : _children)
					child->dismount();
			}
		};

		class VLayout : public LayoutComponent
		{
		public:
			VLayout() : LayoutComponent(UIGroupType::Vertical) {}
		};

		class HLayout : public LayoutComponent
		{
		public:
			HLayout() : LayoutComponent(UIGroupType::Horizontal) {}
		};

		class ListBox : public LayoutComponent
		{
		public:
			ListBox() : LayoutComponent(UIGroupType::List) {}
		};

		template<typename ... Args>
		extern auto vlayout(Args&&... args)
		{
			auto layout = std::make_shared<VLayout>();
			((layout->add(std::forward<Args>(args))), ...);
			return layout;
		}

		template<typename ... Args>
		extern auto hlayout(Args&&... args)
		{
			auto layout = std::make_shared<HLayout>();
			((layout->add(std::forward<Args>(args))), ...);
			return layout;
		}

		class Text : public Component
		{
		public:
			Text()
				: Component(UINodeTag::Text)
			{
			}

			explicit Text(StringView32 text)
				: Component(UINodeTag::Text)
			{
				set_attribute(UIAttribute::Text, text);
			}

			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
		};

		class InputComponent : public Component
		{
		public:
			explicit InputComponent(UIInputType type)
				: Component(UINodeTag::Input)
			{
				set_attribute(UIAttribute::Type, type);
			}
		};

		template<typename T>
		class TypedComponent : public Component
		{
		public:
			explicit TypedComponent(UINodeTag tag) : Component(tag) {}
			TypedComponent(UINodeTag tag, T value) : TypedComponent(tag) { set_value(value); }

			void set_value(T value) { set_attribute(UIAttribute::Value, value); }
			UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }
		};

		template<typename T>
		class TypedInputComponent : public InputComponent
		{
		public:
			explicit TypedInputComponent(UIInputType type) : InputComponent(type) {}
			TypedInputComponent(UIInputType type, T value) : InputComponent(type) { set_value(value); }

			void set_value(T value) { set_attribute(UIAttribute::Value, value); }
			UC_NODISCARD T get_value() const { return get_attribute(UIAttribute::Value).template get<T>(); }
		};

		class TextInput : public InputComponent
		{
		public:
			TextInput() : InputComponent(UIInputType::Text) {}
			explicit TextInput(StringView32 text) : InputComponent(UIInputType::Text) { set_value(text); }

			void set_value(StringView32 value) { set_attribute(UIAttribute::Value, value); }
			UC_NODISCARD String32 get_value() const { return get_attribute(UIAttribute::Value).get_string32(); }
		};

		class Toggle : public TypedInputComponent<Bool>
		{
		public:
			explicit Toggle(Bool value = false)
				: TypedInputComponent(UIInputType::Toggle, value) { }
		};

		class RadioButton : public TypedInputComponent<Bool>
		{
		public:
			explicit RadioButton(Bool value = false)
				: TypedInputComponent(UIInputType::Radio, value) { }
		};

		class Button : public InputComponent
		{
		public:
			Button() : InputComponent(UIInputType::Button) {}
			explicit Button(StringView32 text) : InputComponent(UIInputType::Button) { set_text(text); }

			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }

			void set_click_action(const UIAction& action)
			{
				set_action(UIActionType::OnClick, action);
			}
		};

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		class IntegralInput : public TypedInputComponent<T>
		{
		public:
			explicit IntegralInput()
				: TypedInputComponent<T>(UIInputType::Integer)
			{
				TypedInputComponent<T>::set_attribute(UIAttribute::Min, std::numeric_limits<T>::min());
				TypedInputComponent<T>::set_attribute(UIAttribute::Max, std::numeric_limits<T>::max());
			}

			explicit IntegralInput(T value)
				: IntegralInput()
			{
				set_value(value);
			}

			void set_step(T value) { set_attribute(UIAttribute::Step, value); }
			UC_NODISCARD T get_step() const
			{
				return TypedInputComponent<T>::get_attribute(UIAttribute::Step).template get_integral<T>();
			}
		};

		using IntInput = IntegralInput<Int>;
		using Int64Input = IntegralInput<Int64>;

		template<typename T,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		class FlotingPointInput : public TypedInputComponent<T>
		{
		public:
			explicit FlotingPointInput()
				: TypedInputComponent<T>(UIInputType::Float)
			{
			}

			explicit FlotingPointInput(T value)
				: FlotingPointInput()
			{
				TypedInputComponent<T>::set_value(value);
			}

			explicit FlotingPointInput(T value, T step)
				: FlotingPointInput(value)
			{
				set_step(step);
			}

			void set_step(T value)
			{
				TypedInputComponent<T>::set_attribute(UIAttribute::Step, value);
			}

			UC_NODISCARD T get_step() const
			{
				return TypedInputComponent<T>::get_attribute(UIAttribute::Step).template get_integral<T>();
			}
		};

		using FloatInput = FlotingPointInput<Float>;
		using DoubleInput = FlotingPointInput<Double>;

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		class IntegralRange : public TypedInputComponent<T>
		{
		public:
			explicit IntegralRange(const Range<T>& range, T value)
				: TypedInputComponent<T>(UIInputType::RangeI)
			{
				TypedInputComponent<T>::set_value(value);
				TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
				TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
			}
		};

		using IntRange = IntegralRange<Int>;

		template<typename T,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		class FloatingPointRange : public TypedInputComponent<T>
		{
		public:
			explicit FloatingPointRange(const Range<T>& range, T value)
				: TypedInputComponent<T>(UIInputType::RangeF)
			{
				TypedInputComponent<T>::set_value(value);
				TypedInputComponent<T>::set_attribute(UIAttribute::Min, range.min);
				TypedInputComponent<T>::set_attribute(UIAttribute::Max, range.max);
			}
		};

		using FloatRange = FloatingPointRange<Float>;
		using DoubleRange = FloatingPointRange<Double>;

		class Item : public TypedComponent<Bool>
		{
		public:
			explicit Item(StringView32 text)
				: TypedComponent(UINodeTag::Item)
			{
				set_attribute(UIAttribute::Text, text);
			}

			Item(StringView32 text, Bool value)
				: Item(text)
			{
				set_attribute(UIAttribute::Value, value);
			}

			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }

			void set_click_action(const UIAction& action)
			{
				set_action(UIActionType::OnClick, action);
			}
		};

		class DataModel
		{
		public:
			virtual ~DataModel() = default;
			UC_NODISCARD virtual Size size() const = 0;
		};

		template<typename T>
		class ListDataModel : public DataModel
		{
		public:
			UC_NODISCARD virtual T get_at(Size index) const = 0;
		};

		template<typename TKey, typename TValue>
		class DictionaryDataModel : public DataModel
		{
		public:
			UC_NODISCARD virtual const List<TKey>& keys() const = 0;
			UC_NODISCARD virtual Bool try_get(TKey key, TValue& value) const = 0;
		};

		template<typename TKey, typename TValue>
		class ConstDictionaryDataModel : public DictionaryDataModel<TKey, TValue>
		{
		public:
			explicit ConstDictionaryDataModel(const Dictionary<TKey, TValue>& data)
				: _data(data)
			{
				_keys.reserve(_data.size());
				for (const auto& it : _data)
					_keys.push_back(it->first);
			}

			UC_NODISCARD Size size() const override { return _data.size(); }
			UC_NODISCARD const List<TKey>& keys() const override { return _keys; }
			UC_NODISCARD Bool try_get(TKey key, TValue& value) const override
			{
				const auto it = _data.find(key);
				if (it != _data.end())
				{
					value = it->second;
					return true;
				}

				return true;
			}

		protected:
			Dictionary<TKey, TValue> _data;
			List<TKey> _keys;
		};

		template<typename TKey>
		class ComboBox : public LayoutComponent
		{
			UC_OBJECT_EVENT(changed, TKey);
		public:
			explicit ComboBox(const Dictionary<TKey, String32>& items, TKey value)
				: LayoutComponent(UIGroupType::Combo)
				, _items(items)
				, _value(value)
			{
				update_text();
			}

			void set_value(TKey value)
			{
				if (_value == value)
					return;

				_value = value;
				for (const auto& it : _nodes)
					it.second->set_value(it.first == _value);
				update_text();

				_event_changed.invoke(_value);
			}

			UC_NODISCARD TKey get_value() const { return _value; }

		protected:
			Dictionary<TKey, String32> _items;
			Dictionary<TKey, Shared<Item>> _nodes;
			TKey _value;

			void on_mount() override
			{
				for (const auto& it : _items)
				{
					const auto key = it.first;

					auto item = create<Item>(it.second);
					_nodes[it.first] = item;

					item->set_value(key == _value);
					item->set_click_action([this, key] { set_value(key); });
				}
			}

			void on_dismount() override
			{
				_nodes.clear();
			}

			void update_text()
			{
				auto it = _items.find(_value);
				if (it != _items.end())
					set_attribute(UIAttribute::Value, it->second);
			}
		};

		template<typename T>
		class TableDataModel : public DataModel
		{
		public:
			UC_NODISCARD virtual Size col_count() const = 0;
			UC_NODISCARD virtual StringView32 get_header(Size col) const = 0;
			UC_NODISCARD virtual T get_at(Size row, Size column) const = 0;
		};

		class Table : public LayoutComponent
		{
		public:
			class Header : public LayoutComponent
			{
			public:
				Header() : LayoutComponent(UIGroupType::TableHeader) {}

				void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
				UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
			};

			explicit Table(const Shared<TableDataModel<Shared<Component>>>& model)
				: LayoutComponent(UIGroupType::Table)
				, _model(model)
			{
			}

		protected:
			Shared<TableDataModel<Shared<Component>>> _model;
			Dictionary<Vector2i, Shared<Component>, Vector2SortX<int>> _cells;

			void on_mount() override
			{
				const auto row_count = _model->size();
				const Size col_count = _model->col_count();

				set_attribute(UIAttribute::Value, col_count);

				// Header row
				if (const auto row_layout = create<LayoutComponent>(UIGroupType::TableRow))
				{
					for (unsigned i = 0; i < col_count; i++)
						row_layout->create<Header>()->set_text(_model->get_header(i));
				}

				for (unsigned row = 0; row < row_count; row++)
				{
					if (const auto row_layout = create<LayoutComponent>(UIGroupType::TableRow))
					{
						for (unsigned col = 0; col < col_count; col++)
						{
							if (const auto cell_layout = row_layout->create<LayoutComponent>(UIGroupType::TableCell))
							{
								auto component = _model->get_at(row, col);

								cell_layout->add(component);
								_cells[Vector2i(row, col)] = component;
							}
						}
					}
				}
			}

			void on_dismount() override
			{
				_cells.clear();
			}
		};
	}

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

		static Shared<ui::VLayout> _root;

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

		static const auto model = std::make_shared<TestTableModel>(3);

		Shared<ui::Button> btn_ref;
		Shared<ui::ComboBox<int>> combo_ref;
		Shared<ui::ListBox> list_ref;
		Shared<ui::Button> add_ref;

		_root = ui::vlayout(
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
				ui::ref(ui::ComboBox(items, 0), combo_ref)
			),
			ui::vlayout(
				ui::Text(U"Items"),
				ref(ui::ListBox(), list_ref),
				ref(ui::Button(U"Add"), add_ref)
			),
			ui::vlayout(
				ui::Text(U"Table"),
				ui::Table(model)
			)
		);

		if (btn_ref)
			btn_ref->set_click_action([btn_ref] { btn_ref->set_text(U"Clicked"); });

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
		if (_document && _view && !_position_node.empty())
		{
			auto text = StringBuilder::format("Pos {}, Size {}",
				_view->position().cast<Int>(), _view->size().cast<Int>());
			_document->set_node_attribute(_position_node, UIAttribute::Text, text);
		}

		if (_view)
			_view->render();
	}

	void Example08::draw() const
	{
	}
}