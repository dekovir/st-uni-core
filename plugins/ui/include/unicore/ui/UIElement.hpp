#pragma once
#include "unicore/ui/UIDocument.hpp"

namespace unicore::ui
{
	template <typename T, typename Parameter>
	class StrongType
	{
	public:
		explicit StrongType(T const& value) : _value(value) {}
		explicit StrongType(T&& value) : _value(std::move(value)) {}

		T& get() { return _value; }
		T const& get() const { return _value; }

	private:
		T _value;
	};

#define UNICORE_MAKE_TYPE(name, type) \
	namespace details::tag { struct name {}; } \
	using name = StrongType<type, details::tag::name>;

	class Element
	{
	public:
		virtual ~Element() = default;

		UC_NODISCARD Bool is_mounted() const { return _document != nullptr; }
		UC_NODISCARD UIDocument* document() const { return _document; }
		UC_NODISCARD const UINode& node() const { return _node; }

		void mount(UIDocument& document, const UINode& parent)
		{
			_document = &document;
			_node = on_mount(document, parent);
		}

		void unmount()
		{
			if (!is_mounted()) return;

			_document->remove_node(_node);
			_document = nullptr;
			_node = UINode::Empty;
		}

	protected:
		UIDocument* _document = nullptr;
		UINode _node = UINode::Empty;

		virtual UINode on_mount(UIDocument& document, const UINode& parent) = 0;
	};

	namespace attr
	{
		UNICORE_MAKE_TYPE(Id, String);
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
	class TypedElement : public Element
	{
	public:
		using VariantType = std::variant<attr::Id, attr::Name, attr::Visible, TKeys...>;
		using Params = List<VariantType>;

		TypedElement() = default;

	protected:
		UINodeOptions _options;

		UINode on_mount(UIDocument& document, const UINode& parent) override
		{
			return document.create_node(Tag, _options, parent);
		}

		void apply_params(const Params& params)
		{
			for (const auto& value : params)
				apply_variant(value);
		}

		void apply_variant(const VariantType& value)
		{
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, attr::Id>)
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
	class TypedGroupElement : public TypedElement<UINodeTag::Group, TKeys...>
	{
	public:
		using BaseClass = TypedElement<UINodeTag::Group, TKeys...>;

		template<typename ... Args>
		explicit TypedGroupElement(const typename BaseClass::Params& params, Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::apply_params(params);

			((add(std::forward<Args>(args))), ...);
		}

		template<typename ... Args>
		explicit TypedGroupElement(Args&&... args)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;

			((add(std::forward<Args>(args))), ...);
		}

		void add(const Shared<Element>& element)
		{
			_elements.push_back(element);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		void add(const T& element)
		{
			add(std::make_shared<T>(element));
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<Element, T>>* = nullptr>
		void add(T&& element)
		{
			add(std::make_shared<T>(std::forward<T>(element)));
		}

	protected:
		List<Shared<Element>> _elements;

		UINode on_mount(UIDocument& document, const UINode& parent) override
		{
			auto node = BaseClass::on_mount(document, parent);

			for (const auto& element : _elements)
				element->mount(document, node);

			return node;
		}
	};

	template<UIInputType Type, typename... TKeys>
	class TypedInputElement : public TypedElement<UINodeTag::Input, attr::Value, TKeys...>
	{
	public:
		using BaseClass = TypedElement<UINodeTag::Input, attr::Value, TKeys...>;

		TypedInputElement()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedInputElement(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::apply_params(params);
		}
	};

	template<UIVisualType Type, typename... TKeys>
	class TypedVisualElement : public TypedElement<UINodeTag::Visual, TKeys...>
	{
	public:
		using BaseClass = TypedElement<UINodeTag::Visual, TKeys...>;

		TypedVisualElement()
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
		}

		explicit TypedVisualElement(const typename BaseClass::Params& params)
		{
			BaseClass::_options.attributes[UIAttribute::Type] = Type;
			BaseClass::apply_params(params);
		}
	};

	template<UIGroupType Type, typename ... TValues>
	using GroupElement = TypedGroupElement<Type, TValues...>;

	using VBox = GroupElement<UIGroupType::Vertical>;
	using HBox = GroupElement<UIGroupType::Vertical>;

	template<UIInputType Type, typename ... TValues>
	using InputElement = TypedInputElement<Type, TValues...>;

	template<UIVisualType Type, typename ... TValues>
	using VisualElement = TypedVisualElement<Type, TValues...>;

	using TextElement = VisualElement<UIVisualType::Text, attr::Text>;

	using ButtonElement = InputElement<UIInputType::Button, attr::Text, action::OnClick>;
	using SliderElement = InputElement<UIInputType::RangeF, attr::Min, attr::Max, action::OnChange>;
}