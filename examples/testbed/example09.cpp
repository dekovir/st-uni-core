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

			template<typename ... Args>
			explicit GroupElement(Args&&... args)
			{
				(push_back(std::forward<Args>(args)), ...);
			}

			~GroupElement() override
			{
				for (const auto element : _elements)
				{
					element->~Element();
					Memory::free(element);
				}
				_elements.clear();
			}

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
			void push_back(T&& item)
			{
				auto ptr = Memory::alloc(sizeof(T));
				auto element = new (ptr) T(std::forward<T>(item));
				_elements.push_back(element);
			}

			UC_NODISCARD Element* get_element(Size index)
			{
				if (index < _elements.size())
					return _elements[index];

				return nullptr;
			}

			UC_NODISCARD const Element* get_element(Size index) const
			{
				if (index < _elements.size())
					return _elements[index];

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
			List<Element*> _elements;
		};

		using VLayout = GroupElement<UIGroupType::Vertical>;
		using HLayout = GroupElement<UIGroupType::Horizontal>;

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

		class TextInput : public InputElement<UIInputType::Text>
		{
		public:
			String32 text;

			TextInput() = default;
			explicit TextInput(StringView32 text) : text(text) {}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Value] = text;
			}
		};

		template<UIInputType Type>
		class BoolInputElement : public InputElement<Type>
		{
		public:
			Bool value = false;
			Action<Bool> on_change;

			BoolInputElement() = default;
			explicit BoolInputElement(Bool value) : value(value) {}
			BoolInputElement(Bool value, const Action<Bool>& callback)
				: value(value)
				, on_change(callback)
			{}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Value] = value;
				if (on_change != nullptr)
				{
					auto func = on_change;
					options.actions[UIActionType::OnChange] =
						[func](const Variant& change) { func(change.get_bool()); };
				}
			}
		};

		using Toggle = BoolInputElement<UIInputType::Toggle>;
		using Radio = BoolInputElement<UIInputType::Radio>;

		using ClickCallback = std::function<void()>;

		class Button : public InputElement<UIInputType::Button>
		{
		public:
			String32 label;
			ClickCallback on_click = nullptr;

			explicit Button(StringView32 label) : label(label) {}
			explicit Button(StringView32 label, ClickCallback callback)
				: label(label), on_click(std::move(callback))
			{
				int a = 0;
				a++;
			}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Text] = label;
				if (on_click != nullptr)
					options.actions[UIActionType::OnClick] = on_click;
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

		const auto layout = test::VLayout
		{
			test::HLayout
			{
				test::Text{ U"Text" },
				test::TextInput{ U"Lorem ipsum dolor" },
			},
			test::HLayout
			{
				test::Text{ U"Bool" },
				test::Toggle{true, [this](Bool value) { UC_LOG_DEBUG(logger) << "Toggle value changed to " << value; }},
				test::Radio{}
			},
			test::HLayout
			{
				test::Text{ U"Button" },
				//test::Button{ U"Click" },
				test::Button{ U"Click", [this] { UC_LOG_DEBUG(logger) << "Click"; } },
			},
		};

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