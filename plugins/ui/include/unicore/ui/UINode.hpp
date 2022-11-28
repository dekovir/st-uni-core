#pragma once
#include "unicore/ui/UIAction.hpp"
#include "unicore/system/Index.hpp"

namespace unicore
{
	enum class UIAttribute : UInt8
	{
		Type,
		Value,
		Hidden,
		Width,
		Height,
		Tooltip,
		Text,
		Step,
		Min,
		Max,
	};
	static const List<UIAttribute> UIAttributeKeys = {
		UIAttribute::Type,
		UIAttribute::Value,
		UIAttribute::Hidden,
		UIAttribute::Width,
		UIAttribute::Height,
		UIAttribute::Tooltip,
		UIAttribute::Text,
		UIAttribute::Step,
		UIAttribute::Min,
		UIAttribute::Max,
	};

	using UIAttributeDict = Dictionary<UIAttribute, Variant>;

	enum class UINodeTag : UInt8
	{
		Group,  // Can have children
		Visual, // Display readonly content
		Input,  // Display user editable content
	};

	enum class UIGroupType : UInt8
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

	enum class UIVisualType : UInt8
	{
		Text,      // Display text
		Color,     // Display color
		Image,     // Display image
		Progress,  // Display progress bar
		Separator, // Display horizontal line
		Bullet,    // Display small circle
	};

	enum class UIInputType : UInt8
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

	class UIDocument;

	struct UINodeOptions
	{
		// TODO: Optimize with StringView
		String uid;
		String name;
		UIAttributeDict attributes = {};
		UIActionDict actions = {};
	};

	UNICORE_MAKE_INDEX_WITH_INVALID(UINodeIndex, UInt16);

	class UINode
	{
	public:
		constexpr UINode() noexcept
			: _document(nullptr), _index(UINodeIndex_Invalid) {}
		constexpr UINode(const UIDocument* document, UINodeIndex index) noexcept
			: _document(document), _index(index) {}
		~UINode() = default;

		constexpr UINode(UINode const&) noexcept = default;
		constexpr UINode(UINode&&) noexcept = default;

		constexpr UINode& operator= (UINode const&) noexcept = default;
		constexpr UINode& operator= (UINode&&) noexcept = default;

		UC_NODISCARD constexpr auto document() const { return _document; }
		UC_NODISCARD constexpr auto index() const { return _index; }

		UC_NODISCARD Bool empty() const;
		UC_NODISCARD Bool valid() const;
		UC_NODISCARD UINodeTag tag() const;
		UC_NODISCARD UINode parent() const;

		UC_NODISCARD Optional<String> uid() const;
		UC_NODISCARD Optional<String> name() const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		UC_NODISCARD UIAttributeDict get_attributes() const;

		UC_NODISCARD Variant get(UIAttribute attribute) const;
		UC_NODISCARD Bool has(UIAttribute attribute) const;

		UC_NODISCARD Variant value() const { return get(UIAttribute::Value); }
		UC_NODISCARD Bool hidden() const { return get(UIAttribute::Hidden).get_bool(); }
		UC_NODISCARD Variant type() const { return get(UIAttribute::Type); }
		UC_NODISCARD Variant text() const { return get(UIAttribute::Text); }

		// HIERARCHY ///////////////////////////////////////////////////////////////
		size_t get_children(List<UINode>& children) const;
		UC_NODISCARD List<UINode> get_children() const;
		UC_NODISCARD Size get_children_count() const;

		UC_NODISCARD unsigned get_sibling_index() const;
		UC_NODISCARD UINode get_next_sibling() const;
		UC_NODISCARD UINode get_prev_sibling() const;

		// FIND ////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINode find_by_type(UINodeTag tag) const;
		Size find_all_by_type(UINodeTag tag, List<UINode>& list) const;

		UC_NODISCARD UINode find_by_name(StringView name) const;
		Size find_all_by_name(StringView name, List<UINode>& list) const;

		UC_NODISCARD UINode querry(const Predicate<const UINode&>& predicate) const;
		Size querry_all(const Predicate<const UINode&>& predicate, List<UINode>& list) const;

		static const UINode Empty;

	private:
		const UIDocument* _document;
		UINodeIndex _index;
	};

	static constexpr Bool operator==(const UINode& a, const UINode& b) noexcept
	{
		return a.document() == b.document() && a.index() == b.index();
	}

	static constexpr Bool operator!=(const UINode& a, const UINode& b) noexcept
	{
		return !(a == b);
	}

	static constexpr Bool operator<=(const UINode& a, const UINode& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() <= b.index();

		return a.document() < b.document();
	}

	static constexpr Bool operator>=(const UINode& a, const UINode& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() >= b.index();

		return a.document() > b.document();
	}

	static constexpr Bool operator<(const UINode& a, const UINode& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() < b.index();

		return a.document() < b.document();
	}

	static constexpr Bool operator>(const UINode& a, const UINode& b) noexcept
	{
		if (a.document() == b.document())
			return a.index() > b.index();

		return a.document() > b.document();
	}

	extern UNICODE_STRING_BUILDER_FORMAT(const UINode&);
	extern UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&);
}