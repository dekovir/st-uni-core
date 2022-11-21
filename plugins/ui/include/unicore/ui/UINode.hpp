#pragma once
#include "unicore/ui/UIAction.hpp"
#include "unicore/system/Index.hpp"

namespace unicore
{
	enum class UIAttribute : UInt8
	{
		Type,
		Value,
		Width,
		Height,
		Tooltip,
		Text,
		Step,
		Min,
		Max,
	};

	using UIAttributeDict = Dictionary<UIAttribute, Variant>;

	enum class UINodeTag : UInt8
	{
		Group, // Can have children
		Text,  // Display text
		Color, // Display color ?Move
		Image, // Display image
		Input, // Input values
		Item,  // Selectable ?Move to input
		Progress, // Display progress bar ?Move
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

	enum class UITextType : UInt8
	{
		Normal,
		Heading1,
		Heading2,
		Heading3,
		Heading4,
		Heading5,
		Heading6,
	};

	enum class UIInputType : UInt8
	{
		Text,
		TextArea,
		Toggle,
		Radio,
		Button,
		Image,
		Integer,
		Float,
		RangeI,
		RangeF,
		Vector2,
		Vector3,
		Color3,
		Color4,
	};

	class UIDocument;

	struct UINodeOptions
	{
		StringView uid;
		StringView name;
		Bool visible = true;
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
		UC_NODISCARD Optional<String> style() const;
		UC_NODISCARD Bool visible() const;

		// ATTRIBUTES //////////////////////////////////////////////////////////////
		UC_NODISCARD UIAttributeDict get_attributes() const;

		UC_NODISCARD Variant get(UIAttribute attribute) const;
		UC_NODISCARD Bool has(UIAttribute attribute) const;

		UC_NODISCARD Variant value() const { return get(UIAttribute::Value); }
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