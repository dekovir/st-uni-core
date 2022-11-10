#include "example09.hpp"
#include <utility>
#include "unicore/system/Memory.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/resource/ResourceCache.hpp"
#include "unicore/imgui/ImGuiContext.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example09, "UI 3");

	namespace test
	{
		class Element
		{
		public:
			virtual ~Element() = default;

			virtual UINode create(UIDocument& document, const UINode& parent) const = 0;
		};

#pragma pack(1)
		template<UIGroupType Type>
		class GroupElement : public Element
		{
		public:
			GroupElement() = default;
			GroupElement(GroupElement&& other) = default;
			GroupElement(const GroupElement& other) = default;

			//template<typename ... Args>
			//explicit GroupElement(Args&&... args)
			//{
			//	(add(std::forward<Args>(args)), ...);
			//}

			UINode create(UIDocument& document, const UINode& parent) const override
			{
				UINodeOptions options;
				options.attributes[UIAttribute::Type] = Type;
				const auto node = document.create_node(UINodeTag::Group, options, parent);

				for (unsigned i = 0; i < _elements.size(); i++)
				{
					const auto child = get_element(i);
					child->create(document, node);
				}

				return node;
			}

			UC_NODISCARD Size size() const { return _elements.size(); }
			UC_NODISCARD Bool empty() const { return _elements.empty(); }

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			void add(T&& item)
			{
				_elements.push_back(std::make_shared<T>(std::forward<T>(item)));
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			void add(const T& item)
			{
				_elements.push_back(std::make_shared<T>(item));
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			void add(const Shared<T>& item)
			{
				_elements.push_back(item);
			}

			UC_NODISCARD Element* get_element(Size index)
			{
				if (index < _elements.size())
					return _elements[index].get();

				return nullptr;
			}

			UC_NODISCARD const Element* get_element(Size index) const
			{
				if (index < _elements.size())
					return _elements[index].get();

				return nullptr;
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			UC_NODISCARD T* get(Size index)
			{
				if (auto element = get_element(index))
					return dynamic_cast<T*>(element);
				return nullptr;
			}

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			UC_NODISCARD const T* get(Size index) const
			{
				if (auto element = get_element(index))
					return dynamic_cast<const T*>(element);
				return nullptr;
			}

		protected:
			List<Shared<Element>> _elements;
		};

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		extern Shared<T> shared(T&& element)
		{
			return std::make_shared<T>(std::forward<T>(element));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		extern Shared<T> ref(T&& element, Shared<T>& value)
		{
			value = std::make_shared<T>(std::forward<T>(element));
			return value;
		}

		template<typename ... Args>
		extern auto vlayout(Args&&... args)
		{
			GroupElement<UIGroupType::Vertical> layout;
			(layout.add(std::forward<Args>(args)), ...);
			return layout;
		}

		template<typename ... Args>
		extern auto hlayout(Args&&... args)
		{
			GroupElement<UIGroupType::Horizontal> layout;
			(layout.add(std::forward<Args>(args)), ...);
			return layout;
		}

		template<typename ... Args>
		extern auto list(Args&&... args)
		{
			GroupElement<UIGroupType::List> layout;
			(layout.add(std::forward<Args>(args)), ...);
			return layout;
		}

		template<typename ... Args>
		extern auto tree(Args&&... args)
		{
			GroupElement<UIGroupType::Tree> layout;
			(layout.add(std::forward<Args>(args)), ...);
			return layout;
		}

		class Text : public Element
		{
		public:
			String32 text;

			explicit Text(StringView32 value)
				: text(value) {}

			UC_NODISCARD UINode create(UIDocument& document, const UINode& parent) const override
			{
				UINodeOptions options;
				options.attributes[UIAttribute::Text] = text;
				return document.create_node(UINodeTag::Text, options, parent);
			}
		};

		class Item : public Element
		{
		public:
			String32 text;
			Bool value;

			explicit Item(String32 text, Bool value = false)
				: text(std::move(text)), value(value) {}

			UC_NODISCARD UINode create(UIDocument& document, const UINode& parent) const override
			{
				UINodeOptions options;
				options.attributes[UIAttribute::Value] = value;
				options.attributes[UIAttribute::Text] = text;
				return document.create_node(UINodeTag::Item, options, parent);
			}
		};

		template<UIInputType Type>
		class InputElement : public Element
		{
		public:
			UINode create(UIDocument& document, const UINode& parent) const override
			{
				UINodeOptions options;
				apply_options(options);
				options.attributes[UIAttribute::Type] = Type;
				return document.create_node(UINodeTag::Input, options, parent);
			}

		protected:
			virtual void apply_options(UINodeOptions& options) const = 0;
		};

		using DefaultCallback = std::function<void()>;

		template<typename T>
		using ValueCallback = std::function<void(T)>;

		template<typename T>
		using InputCallback = StdVariant<DefaultCallback, ValueCallback<T>>;

		template<UIInputType Type, typename TValue>
		class ValueInputElement : public InputElement<Type>
		{
		public:
			using CallbackType = InputCallback<TValue>;

			TValue value = {};
			Optional<CallbackType> on_change;

			ValueInputElement() = default;

			explicit ValueInputElement(TValue value)
				: value(std::move(value)) {}

			ValueInputElement(TValue value, const CallbackType& callback)
				: value(std::move(value)), on_change(callback)
			{}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Value] = value;
				if (on_change.has_value())
				{
					if (const auto value_action = std::get_if<ValueCallback<TValue>>(&on_change.value()))
					{
						auto func = *value_action;
						options.actions[UIActionType::OnChange] =
							[func](const Variant& change) { func(change.get<TValue>()); };
					}
					else if (const auto default_action = std::get_if<DefaultCallback>(&on_change.value()))
						options.actions[UIActionType::OnChange] = *default_action;
				}
			}
		};

		using Toggle = ValueInputElement<UIInputType::Toggle, Bool>;
		using Radio = ValueInputElement<UIInputType::Radio, Bool>;
		using TextInput = ValueInputElement<UIInputType::Text, String32>;

		class Button : public InputElement<UIInputType::Button>
		{
		public:
			String32 label;
			DefaultCallback on_click = nullptr;

			explicit Button(StringView32 label) : label(label) {}
			explicit Button(StringView32 label, DefaultCallback callback)
				: label(label), on_click(std::move(callback))
			{
			}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Text] = label;
				if (on_click != nullptr)
					options.actions[UIActionType::OnClick] = on_click;
			}
		};

		class Component : public Element
		{
		public:
			//virtual ~Component() = default;

			UC_NODISCARD virtual Shared<Element> render() = 0;

			void update()
			{
				if (_need_rebuild)
				{
					_element = render();
					_need_rebuild = false;
				}

				on_update();
			}

			void rebuild()
			{
				_need_rebuild = true;
			}

			UINode create(UIDocument& document, const UINode& parent) const override
			{
				if (_element != nullptr)
					return _element->create(document, parent);

				return UINode::Empty;
			}

		protected:
			Shared<Element> _element;

			virtual void on_update() {}

		private:
			Bool _need_rebuild = true;
		};

		class Property : public Component
		{
		public:
			explicit Property(StringView32 name)
				: _name(name)
			{}

			UC_NODISCARD Shared<Element> render() override
			{
				return shared(hlayout(
					Text(_name),
					TextInput()
				));
			}

		protected:
			String32 _name;
		};

		class TestCompoent : public Component
		{
		public:
			UC_NODISCARD Shared<Element> render() override
			{
				auto list_element = list();
				for (const auto& item : items)
					list_element.add(Item(item));

				return shared(vlayout(
					Text(U"Items:"),
					list_element,
					Button(U"Click", [this] { on_add(); })
				));
			}

		protected:
			List<String32> items;

			void on_add()
			{
				items.push_back(StringBuilder::format(U"Item {}", items.size() + 1));
				rebuild();
			}
		};
	}

	Example09::Example09(const ExampleContext& context)
		: Example(context)
		, _context(context.imgui)
	{
	}

	void Example09::load(IResourceCache& resources)
	{
		Example::load(resources);

		const auto size = renderer.screen_size();

		_document = std::make_shared<UIDocument>(&logger);
		_view = std::make_shared<UIViewImGui>(_context, logger);
		_view->set_document(_document);
		_view->set_title(U"Test UI");
		_view->set_size(Vector2f(300, 0));
		_view->set_position(Vector2f(size.x / 2 - 150, 50));

		Shared<test::GroupElement<UIGroupType::List>> list_ref;
		Shared<test::Radio> radio_ref;

		const auto layout = test::vlayout(
			test::hlayout(
				test::Text(U"Text"),
				test::TextInput(U"Lorem ipsum dolor")
			),
			test::hlayout(
				test::Text(U"Text"),
				test::Toggle(true, [this] { UC_LOG_DEBUG(logger) << "Test"; }),
				test::ref(test::Radio(), radio_ref)
			),
			test::hlayout(
				test::Text(U"Text"),
				test::Button(U"Click", [this] { UC_LOG_DEBUG(logger) << "Test"; })
			),
			test::Text(U"Items"),
			test::ref(test::list(), list_ref),
			test::Button(U"Click", [this] {})
		);

		if (radio_ref)
			radio_ref->on_change = [this] { UC_LOG_DEBUG(logger) << "Test"; };

		layout.create(*_document, UINode::Empty);
	}

	void Example09::update()
	{
		if (_view)
			_view->render();
	}

	void Example09::draw() const
	{
	}
}