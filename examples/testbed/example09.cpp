#include "example09.hpp"
#include <utility>
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

		class ElementList : public Element
		{
		public:
			ElementList() = default;

			template<typename ... Args>
			explicit ElementList(Args&&... args)
			{
				(push_back(std::forward<Args>(args)), ...);
			}

			UC_NODISCARD Size size() const { return _items.size(); }
			UC_NODISCARD Bool empty() const { return _items.empty(); }

			UC_NODISCARD Size bytes_size() const { return _data.size(); }

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			void push_back(T&& item)
			{
				ItemInfo info;
				info.offset = _data.size();
				info.size = sizeof(T);
				_items.push_back(info);

				_data.resize(_data.size() + info.size);
				new (_data.data() + info.offset) T(item);
			}

			UC_NODISCARD Element* get_element(Size index)
			{
				if (index < _items.size())
				{
					const auto& info = _items[index];
					return reinterpret_cast<Element*>(_data.data() + info.offset);
				}

				return nullptr;
			}

			UC_NODISCARD const Element* get_element(Size index) const
			{
				if (index < _items.size())
				{
					const auto& info = _items[index];
					return reinterpret_cast<const Element*>(_data.data() + info.offset);
				}

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
			struct ItemInfo
			{
				Size offset;
				Size size;
			};

			List<uint8_t> _data;
			List<ItemInfo> _items;
		};

#pragma pack(1)
		template<UIGroupType Type>
		class GroupElement : public Element
		{
		public:
			GroupElement() = default;

			template<typename ... Args>
			explicit GroupElement(Args&&... args)
			{
				(push_back(std::forward<Args>(args)), ...);
			}

			UC_NODISCARD Element* get_element(Size index)
			{
				if (index < _items.size())
				{
					const auto& info = _items[index];
					return reinterpret_cast<Element*>(_data.data() + info.offset);
				}

				return nullptr;
			}

			UC_NODISCARD const Element* get_element(Size index) const
			{
				if (index < _items.size())
				{
					const auto& info = _items[index];
					return reinterpret_cast<const Element*>(_data.data() + info.offset);
				}

				return nullptr;
			}

			UINode create(UIDocument& document, const UINode& parent) const override
			{
				UINodeOptions options;
				options.attributes[UIAttribute::Type] = Type;
				const auto node = document.create_node(UINodeTag::Group, options, parent);

				for (unsigned i = 0; i < _items.size(); i++)
				{
					const auto child = get_element(i);
					child->create(document, node);
				}

				return node;
			}

		protected:
			struct ItemInfo
			{
				Size offset;
				Size size;
			};

			List<uint8_t> _data;
			List<ItemInfo> _items;

			template<typename T,
				std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
			void push_back(const T& item)
			{
				ItemInfo info;
				info.offset = _data.size();
				info.size = sizeof(T);
				_items.push_back(info);

				_data.resize(_data.size() + sizeof(T));
				new (_data.data() + info.offset) T(item);
			}
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

			BoolInputElement() = default;
			explicit BoolInputElement(Bool value) : value(value) {}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Value] = value;
			}
		};

		using Toggle = BoolInputElement<UIInputType::Toggle>;
		using Radio = BoolInputElement<UIInputType::Radio>;

		class Button : public InputElement<UIInputType::Button>
		{
		public:
			String32 label;

			Button() = default;
			explicit Button(StringView32 label) : label(label) {}

		protected:
			void apply_options(UINodeOptions& options) const override
			{
				options.attributes[UIAttribute::Text] = label;
			}
		};

		/*struct Item1 : Element
		{
			int a;
			explicit Item1(int value) : a(value) {}
		};

		struct Item2 : Element
		{
			String text;
			explicit Item2(StringView value) : text(value) {}
		};

		struct Item3 : Element
		{
			Bool value;
			explicit Item3(Bool value) : value(value) {}
		};*/
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
				test::Toggle{true},
				test::Radio{}
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