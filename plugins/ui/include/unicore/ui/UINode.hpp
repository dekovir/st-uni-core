#pragma once
#include "UINode.hpp"
#include "unicore/ui/UIAttribute.hpp"

namespace unicore
{
	// ACTIONS
	class UINode;

	using UIActionDefault = std::function<void()>;
	using UIActionNodeDefault = std::function<void(const UINode&)>;

	using UIActionValue = std::function<void(const Variant&)>;
	using UIActionNodeValue = std::function<void(const UINode&, const Variant&)>;

	using UIAction = StdVariant<UIActionDefault, UIActionNodeDefault, UIActionValue, UIActionNodeValue>;

	enum class UIActionType : uint8_t
	{
		OnClick,
		OnChange, // Value changed
		OnMouseEnter, // Mouse cursor enters
		OnMouseLeave, // Mouse cursor enters
	};
	using UIActionDict = Dictionary<UIActionType, UIAction>;

	// NODE
	enum class UINodeType
	{
		Group,
		Text,
		Image,
		Button,
		Input,
		Slider,
		Toggle,
		Tooltip,
		List,
		Item,
		Tree,
		Combo,
		Table,
		TableHeader,
		TableRow,
		TableCell,
		Progress,
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

	class UINode
	{
	public:
		using IndexType = UInt16;
		static constexpr IndexType InvalidIndex = std::numeric_limits<IndexType>::max();

		constexpr UINode() noexcept
			: _document(nullptr), _index(InvalidIndex) {}
		constexpr UINode(const UIDocument* document, IndexType index) noexcept
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
		UC_NODISCARD UINodeType type() const;
		UC_NODISCARD UINode parent() const;

		UC_NODISCARD Optional<String> uid() const;
		UC_NODISCARD Optional<String> name() const;
		UC_NODISCARD Optional<String> style() const;
		UC_NODISCARD Bool visible() const;

		UC_NODISCARD UIAttributeDict get_attributes() const;
		UC_NODISCARD UIActionDict get_actions() const;

		UC_NODISCARD Variant attribute(UIAttributeType type) const;

		UC_NODISCARD Variant value() const { return attribute(UIAttributeType::Value); }
		UC_NODISCARD Variant text() const { return attribute(UIAttributeType::Text); }

		UC_NODISCARD Optional<UIAction> action(UIActionType type) const;

		size_t get_children(List<UINode>& children) const;
		UC_NODISCARD List<UINode> get_children() const;
		UC_NODISCARD Size get_children_count() const;

		UC_NODISCARD unsigned get_sibling_index() const;
		UC_NODISCARD UINode get_next_sibling() const;
		UC_NODISCARD UINode get_prev_sibling() const;

		// FIND //////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINode find_by_type(UINodeType type) const;
		Size find_all_by_type(UINodeType type, List<UINode>& list) const;

		UC_NODISCARD UINode find_by_name(StringView name) const;
		Size find_all_by_name(StringView name, List<UINode>& list) const;

		UC_NODISCARD UINode querry(const Predicate<const UINode&>& predicate) const;
		Size querry_all(const Predicate<const UINode&>& predicate, List<UINode>& list) const;

		static const UINode Empty;

	private:
		const UIDocument* _document;
		IndexType _index;
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
}