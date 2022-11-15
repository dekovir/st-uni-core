#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "unicore/ui/UIDataModel.hpp"

namespace unicore::ui
{
	class Component
	{
	public:
		explicit Component(UINodeTag tag);
		virtual ~Component() = default;

		UC_NODISCARD Bool is_mounted() const { return !_node.empty(); }

		UC_NODISCARD UINodeTag tag() const { return _tag; }
		UC_NODISCARD const UINode& node() const { return _node; }

		UC_NODISCARD UIDocument* document() { return _document; }
		UC_NODISCARD const UIDocument* document() const { return _document; }

		void mount(UIDocument& document, const UINode& parent);
		void dismount();

	protected:
		virtual void on_mount() {}
		virtual void on_dismount() {}

		UC_NODISCARD Variant get_attribute(UIAttribute type) const { return _node.get(type); }

		void set_attribute(UIAttribute type, const Variant& value);
		void set_action(UIActionType type, const UIAction& action);

	private:
		const UINodeTag _tag;
		UINodeOptions _options;

		UIDocument* _document = nullptr;
		UINode _node = UINode::Empty;
	};

	namespace sfinae
	{
		template<typename T>
		inline constexpr Bool is_component_v =
			std::is_base_of_v<Component, T> ||
			std::is_convertible_v<T, Shared<Component>>;

		template<typename ... T>
		inline constexpr Bool all_is_component_v = (... && is_component_v<T>);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& value)
	{
		value = std::make_shared<T>(std::forward<T>(element));
		return value;
	}

	class Group : public Component
	{
	public:
		explicit Group(UIGroupType type)
			: Component(UINodeTag::Group)
		{
			set_attribute(UIAttribute::Type, type);
		}

		UC_NODISCARD Size size() const { return _children.size(); }

		template<typename T = Component,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(const Shared<T>& component)
		{
			// TODO: Create/destroy
			_children.push_back(component);
			if (is_mounted())
				component->mount(*document(), node());

			return component;
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(const T& component)
		{
			return add(std::make_shared<T>(component));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Component, T>>* = nullptr>
		auto add(T&& component)
		{
			return add(std::make_shared<T>(std::forward<T>(component)));
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

	template<UIGroupType Type>
	class TypedGroup : public Group
	{
	public:
		TypedGroup() : Group(Type) {}

		template<typename ... Args,
			std::enable_if_t<sfinae::all_is_component_v<Args...>>* = nullptr>
		explicit TypedGroup(Args&&... args)
			: TypedGroup()
		{
			((add(std::forward<Args>(args))), ...);
		}
	};

	using vlayout = TypedGroup<UIGroupType::Vertical>;
	using hlayout = TypedGroup<UIGroupType::Horizontal>;

	using child = TypedGroup<UIGroupType::Child>;
	using list_box = TypedGroup<UIGroupType::List>;
	using tree = TypedGroup<UIGroupType::Tree>;
	using combo = TypedGroup<UIGroupType::Combo>;
	using flex = TypedGroup<UIGroupType::Flex>;

	using table = TypedGroup<UIGroupType::Table>;
	using table_header = TypedGroup<UIGroupType::TableHeader>;
	using table_row = TypedGroup<UIGroupType::TableRow>;
	using table_cell = TypedGroup<UIGroupType::TableCell>;

	using popup = TypedGroup<UIGroupType::Popup>;
	using tooltip = TypedGroup<UIGroupType::Tooltip>;
	using modal = TypedGroup<UIGroupType::Modal>;

	class Text : public Component
	{
	public:
		Text() : Component(UINodeTag::Text) {}

		explicit Text(StringView32 text) : Text()
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

	template<typename TKey>
	class ComboBox : public Group
	{
		UC_OBJECT_EVENT(changed, TKey);
	public:
		explicit ComboBox(const Shared<DictionaryDataModel<TKey, String32>>& model, TKey value)
			: Group(UIGroupType::Combo)
			, _model(model)
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
		Shared<DictionaryDataModel<TKey, String32>> _model;
		Dictionary<TKey, Shared<Item>> _nodes;
		TKey _value;

		void on_mount() override
		{
			for (const auto& key : _model->keys())
			{
				String32 text;
				if (_model->try_get(key, text))
				{
					auto item = add(Item(text));
					_nodes[key] = item;

					item->set_value(key == _value);
					item->set_click_action([this, key] { set_value(key); });
				}
			}
		}

		void on_dismount() override
		{
			_nodes.clear();
		}

		void update_text()
		{
			String32 text;
			if (_model->try_get(_value, text))
				set_attribute(UIAttribute::Value, text);
		}
	};

	class Table : public Group
	{
	public:
		class Header : public TypedGroup<UIGroupType::TableHeader>
		{
		public:
			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
		};

		explicit Table(const Shared<TableDataModel<Shared<Component>>>& model)
			: Group(UIGroupType::Table)
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
			if (const auto row_layout = add(table_row()))
			{
				for (unsigned i = 0; i < col_count; i++)
					row_layout->add(Header())->set_text(_model->get_header(i));
			}

			for (unsigned row = 0; row < row_count; row++)
			{
				if (const auto row_layout = add(table_row()))
				{
					for (unsigned col = 0; col < col_count; col++)
					{
						if (const auto cell_layout = row_layout->add(table_cell()))
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