#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/system/Variant.hpp"

namespace unicore::remoteui
{
	enum class Attribute : UInt8
	{
		Type,
		Name,
		Value,
		Hidden,
		Disabled,
		Width,
		Height,
		Tooltip,
		Text,
		Step,
		Min,
		Max,
	};
	static const List<Attribute> AttributeKeys = {
		Attribute::Type,
		Attribute::Name,
		Attribute::Value,
		Attribute::Hidden,
		Attribute::Disabled,
		Attribute::Width,
		Attribute::Height,
		Attribute::Tooltip,
		Attribute::Text,
		Attribute::Step,
		Attribute::Min,
		Attribute::Max,
	};

	using AttributeDict = Dictionary<Attribute, Variant>;

	enum class ElementTag : UInt8
	{
		Group,  // Can have children
		Visual, // Display readonly content
		Input,  // Display user editable content
	};

	enum class GroupType : UInt8
	{
		Vertical,
		Horizontal,
		Child,
		List,
		Tree,
		Combo,
		Flex,
		Table,
		TableHeader,
		TableRow,
		TableCell,
		Popup,
		Tooltip,
		Modal,
	};

	enum class VisualType : UInt8
	{
		Text,      // Display text
		Color,     // Display color
		Image,     // Display image
		Progress,  // Display progress bar
		Separator, // Display horizontal line
		Bullet,    // Display small circle
	};

	enum class InputType : UInt8
	{
		Text,     // Single line text edit
		TextArea, // Multi line text edit
		Toggle,   // Checkbox
		Radio,    // Radio button
		Button,   // Button
		Item,     // Selectable
		Image,    // Image button
		Integer,  // Number (Integer)
		Float,    // Number (Float)
		RangeI,   // Slider (Integer)
		RangeF,   // Slider (Float)
		Color3,   // Color without alpha
		Color4,   // Color with alpha
	};

	class Element;

	using ActionDefault = Action<void>;
	using UIActionValue = Action<const Variant&>;
	using UIAction = StdVariant<ActionDefault, UIActionValue>;

	enum class UIActionType : uint8_t
	{
		OnClick,
		OnChange, // Value changed
		OnMouseEnter, // Mouse cursor enters node TODO: Remove?
		OnMouseLeave, // Mouse cursor leaves node TODO: Remove?
	};
	using UIActionDict = Dictionary<UIActionType, UIAction>;

	struct ElementOptions
	{
		AttributeDict attributes = {};
		// TODO: Move to attributes?
		UIActionDict actions = {};
	};

	UNICORE_MAKE_INDEX_WITH_INVALID(ElementIndex, UInt16);

	class Document;

	class Element
	{
	public:
		constexpr Element() noexcept
			: _document(nullptr), _index(ElementIndex_Invalid) {}
		constexpr Element(const Document* document, ElementIndex index) noexcept
			: _document(document), _index(index) {}
		~Element() = default;

		constexpr Element(Element const&) noexcept = default;
		constexpr Element(Element&&) noexcept = default;

		constexpr Element& operator= (Element const&) noexcept = default;
		constexpr Element& operator= (Element&&) noexcept = default;

		UC_NODISCARD constexpr auto document() const { return _document; }
		UC_NODISCARD constexpr auto index() const { return _index; }

		UC_NODISCARD Bool empty() const;
		UC_NODISCARD Bool valid() const;
		UC_NODISCARD ElementTag tag() const;
		UC_NODISCARD Element parent() const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		UC_NODISCARD AttributeDict get_attributes() const;

		UC_NODISCARD Variant get(Attribute attribute) const;
		UC_NODISCARD Bool has(Attribute attribute) const;

		UC_NODISCARD Variant type() const { return get(Attribute::Type); }
		UC_NODISCARD StringView name() const;
		UC_NODISCARD Variant value() const { return get(Attribute::Value); }
		UC_NODISCARD Bool hidden() const { return get(Attribute::Hidden).get_bool(); }
		UC_NODISCARD Bool disabled() const { return get(Attribute::Disabled).get_bool(); }
		UC_NODISCARD Variant text() const { return get(Attribute::Text); }

		// HIERARCHY ///////////////////////////////////////////////////////////////
		size_t get_children(List<Element>& children) const;
		UC_NODISCARD List<Element> get_children() const;
		UC_NODISCARD Size get_children_count() const;
		UC_NODISCARD Element get_children(int index) const;

		UC_NODISCARD int get_sibling_index() const;

		UC_NODISCARD Element get_next_sibling() const;
		UC_NODISCARD Element get_prev_sibling() const;

		// FIND ////////////////////////////////////////////////////////////////////
		UC_NODISCARD Element find_by_type(ElementTag tag) const;
		Size find_all_by_type(ElementTag tag, List<Element>& list) const;

		UC_NODISCARD Element find_by_name(StringView name) const;
		Size find_all_by_name(StringView name, List<Element>& list) const;

		UC_NODISCARD Element query(const Predicate<const Element&>& predicate) const;
		Size query_all(const Predicate<const Element&>& predicate, List<Element>& list) const;

		static const Element Empty;

	private:
		const Document* _document;
		ElementIndex _index;
	};

	static constexpr Bool operator==(const Element& a, const Element& b) noexcept
	{
		return a.document() == b.document() && a.index() == b.index();
	}

	static constexpr Bool operator!=(const Element& a, const Element& b) noexcept
	{
		return !(a == b);
	}

	static constexpr Bool operator<=(const Element& a, const Element& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() <= b.index();

		return a.document() < b.document();
	}

	static constexpr Bool operator>=(const Element& a, const Element& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() >= b.index();

		return a.document() > b.document();
	}

	static constexpr Bool operator<(const Element& a, const Element& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() < b.index();

		return a.document() < b.document();
	}

	static constexpr Bool operator>(const Element& a, const Element& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() > b.index();

		return a.document() > b.document();
	}

	extern UNICODE_STRING_BUILDER_FORMAT(const Element&);
	extern UNICODE_STRING_BUILDER_FORMAT(const ElementIndex&);
}
