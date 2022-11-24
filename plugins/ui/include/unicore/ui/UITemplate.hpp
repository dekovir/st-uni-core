#pragma once
#include "unicore/ui/UIDocument.hpp"

namespace unicore::ui
{
	template <typename T, typename Parameter>
	class StrongType
	{
	public:
		StrongType() : _value({}) {}
		explicit StrongType(T const& value) : _value(value) {}
		explicit StrongType(T&& value) : _value(std::move(value)) {}

		T& get() { return _value; }
		UC_NODISCARD T const& get() const { return _value; }

	private:
		T _value;
	};

#define UNICORE_MAKE_TYPE(name, type) \
	namespace details::tag { struct name {}; } \
	using name = StrongType<type, details::tag::name>;

	class Template
	{
	public:
		virtual ~Template() = default;

		virtual UINode create(UIDocument& document, const UINode& parent) = 0;
		virtual Bool apply_to(UIDocument& document, const UINode& node) = 0;
	};

	namespace attr
	{
		UNICORE_MAKE_TYPE(Uid, String);
		UNICORE_MAKE_TYPE(Name, String);
		UNICORE_MAKE_TYPE(Visible, Bool);

		UNICORE_MAKE_TYPE(Type, UInt8);
		UNICORE_MAKE_TYPE(Value, Variant);

		UNICORE_MAKE_TYPE(Width, Variant);
		UNICORE_MAKE_TYPE(Height, Variant);

		UNICORE_MAKE_TYPE(Tooltip, String32);
		UNICORE_MAKE_TYPE(Text, String32);

		UNICORE_MAKE_TYPE(Step, Variant);

		UNICORE_MAKE_TYPE(Min, Variant);
		UNICORE_MAKE_TYPE(Max, Variant);
	}

	namespace action
	{
		UNICORE_MAKE_TYPE(OnClick, UIActionDefault);
		UNICORE_MAKE_TYPE(OnChange, UIAction);
	}

	template<UINodeTag Tag, typename... TKeys>
	class TypedTemplate : public Template
	{
	public:
		using VariantType = std::variant<attr::Uid, attr::Name, attr::Visible, TKeys...>;
		using Params = List<VariantType>;

		TypedTemplate() = default;

		void set_params(const Params& params)
		{
			for (const auto& value : params)
				apply_variant(value);
		}

		UINode create(UIDocument& document, const UINode& parent) override
		{
			return document.create_node(Tag, _options, parent);
		}

		Bool apply_to(UIDocument& document, const UINode& node) override
		{
			if (node.tag() != Tag)
				return false;

			return document.apply_options(node, _options);
		}

	protected:
		UINodeOptions _options;

		void apply_variant(const VariantType& value)
		{
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, attr::Uid>)
				_options.uid = arg.get();
			else if constexpr (std::is_same_v<T, attr::Name>)
				_options.name = arg.get();
			else if constexpr (std::is_same_v<T, attr::Visible>)
				_options.visible = arg.get();

			else if constexpr (std::is_same_v<T, attr::Type>)
				_options.attributes[UIAttribute::Type] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Value>)
				_options.attributes[UIAttribute::Value] = arg.get();

			else if constexpr (std::is_same_v<T, attr::Width>)
				_options.attributes[UIAttribute::Width] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Height>)
				_options.attributes[UIAttribute::Height] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Tooltip>)

				_options.attributes[UIAttribute::Tooltip] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Text>)
				_options.attributes[UIAttribute::Text] = arg.get();

			else if constexpr (std::is_same_v<T, attr::Step>)
				_options.attributes[UIAttribute::Step] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Min>)
				_options.attributes[UIAttribute::Min] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Max>)
				_options.attributes[UIAttribute::Max] = arg.get();

			else if constexpr (std::is_same_v<T, action::OnClick>)
				_options.actions[UIActionType::OnClick] = arg.get();
			else if constexpr (std::is_same_v<T, action::OnChange>)
				_options.actions[UIActionType::OnChange] = arg.get();
				}, value);
		}
	};

	template<UIGroupType Type, typename... TKeys>
	class TypedGroupTemplate : public TypedTemplate<UINodeTag::Group, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Group, TKeys...>;

		template<typename ... Args>
		explicit TypedGroupTemplate(const typename BaseClass::Params& params, Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);

			((add(std::forward<Args>(args))), ...);
		}

		template<typename ... Args>
		explicit TypedGroupTemplate(Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;

			((add(std::forward<Args>(args))), ...);
		}

		void add(const Shared<Template>& element)
		{
			_elements.push_back(element);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
		void add(const T& element)
		{
			add(std::make_shared<T>(element));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
		void add(T&& element)
		{
			add(std::make_shared<T>(std::forward<T>(element)));
		}

		UINode create(UIDocument& document, const UINode& parent) override
		{
			auto node = BaseClass::create(document, parent);

			for (const auto& element : _elements)
				element->create(document, node);

			return node;
		}

		Bool apply_to(UIDocument& document, const UINode& node) override
		{
			if (!BaseClass::apply_to(document, node))
				return false;

			// TODO: Optimize
			if (document.get_node_children_count(node) != _elements.size())
				return false;

			for (unsigned i = 0; i < _elements.size(); i++)
			{
				const auto& element = _elements[i];

				auto child = document.get_node_child(node, i);
				if (child.empty()) return false;

				if (!element->apply_to(document, child))
					return false;
			}

			return true;
		}

	protected:
		List<Shared<Template>> _elements;
	};

	template<UIInputType Type, typename... TKeys>
	class TypedInputTemplate : public TypedTemplate<UINodeTag::Input, attr::Value, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Input, attr::Value, TKeys...>;

		TypedInputTemplate()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedInputTemplate(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);
		}
	};

	template<UIVisualType Type, typename... TKeys>
	class TypedVisualTemplate : public TypedTemplate<UINodeTag::Visual, TKeys...>
	{
	public:
		using BaseClass = TypedTemplate<UINodeTag::Visual, TKeys...>;

		TypedVisualTemplate()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedVisualTemplate(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::set_params(params);
		}
	};

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(const T& element)
	{
		return std::make_shared<T>(element);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(const T& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}

	template<UIGroupType Type, typename ... TValues>
	using GroupTemplate = TypedGroupTemplate<Type, TValues...>;

	using VBox = GroupTemplate<UIGroupType::Vertical>;
	using HBox = GroupTemplate<UIGroupType::Vertical>;

	template<UIInputType Type, typename ... TValues>
	using InputTemplate = TypedInputTemplate<Type, TValues...>;

	template<UIVisualType Type, typename ... TValues>
	using VisualTemplate = TypedVisualTemplate<Type, TValues...>;

	using TextTemplate = VisualTemplate<UIVisualType::Text, attr::Text>;

	using ButtonTemplate = InputTemplate<UIInputType::Button, attr::Text, action::OnClick>;
	using SliderTemplate = InputTemplate<UIInputType::RangeF, attr::Min, attr::Max, action::OnChange>;

	class Element
	{
	public:
		virtual ~Element() = default;
		virtual Shared<Template> render() = 0;
	};

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	class TypedElement : public Element
	{
	public:
		Shared<Template> render() override
		{
			auto temp = ptr(T());
			apply_params(*temp);
			return temp;
		}

	protected:
		String _name;
		Bool _visible = true;

		virtual void apply_params(T& temp)
		{
			if (!_name.empty())
				temp.set_params({ attr::Name(_name) });
			temp.set_params({ attr::Visible(_visible) });
		}
	};

	class ButtonElement : public TypedElement<ButtonTemplate>
	{
		UC_OBJECT_EVENT(clicked, Bool);
	public:

	protected:
		String32 _text;

		void apply_params(ButtonTemplate& temp) override
		{
			TypedElement<ButtonTemplate>::apply_params(temp);

			if (!_text.empty())
				temp.set_params({ attr::Text(_text) });

			if (!_event_clicked.empty())
				temp.set_params({ action::OnClick([&] { _event_clicked.invoke(true); }) });
		}
	};
}