#pragma once
#include "UINode.hpp"
#include "unicore/system/Variant.hpp"

namespace unicore
{
	// ATTRIBUTES
	enum class UIAttributeType
	{
		Value,
		Width,
		Height,
		Tooltip,
		//Style,
		Text,
		MinValue,
		MaxValue,
	};

	enum class UILayout
	{
		None,
		Vertical,
		Horizontal,
	};

	using UIAttributeDict = Dictionary<UIAttributeType, Variant>;

	// ACTIONS
	class UINode;
	using UIActionDefault = std::function<void()>;
	using UIActionNodeDefault = std::function<void(const UINode&)>;

	using UIActionValue = std::function<void(const Variant&)>;
	using UIActionNodeValue = std::function<void(const UINode&, const Variant&)>;

	using UIAction = StdVariant<UIActionDefault, UIActionNodeDefault, UIActionValue, UIActionNodeValue>;

	enum class UIActionType : uint8_t
	{
		OnLoad,
		OnClick,
		OnChange,
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
		Item,
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

		UINode();
		UINode(const UIDocument* document, IndexType index);
		~UINode() = default;

		UC_TYPE_DEFAULT_MOVE_COPY(UINode);

		UC_NODISCARD auto document() const { return _document; }
		UC_NODISCARD auto index() const { return _index; }

		UC_NODISCARD Bool valid() const;
		UC_NODISCARD UINodeType type() const;
		UC_NODISCARD Optional<UINode> parent() const;

		UC_NODISCARD const String& uid() const;
		UC_NODISCARD const String& name() const;
		UC_NODISCARD Bool visible() const;

		UC_NODISCARD const UIAttributeDict& attributes() const;
		UC_NODISCARD const UIActionDict& actions() const;

		UC_NODISCARD Variant get_attribute(UIAttributeType type) const;

		UC_NODISCARD Variant get_value() const { return get_attribute(UIAttributeType::Value); }

		UC_NODISCARD Optional<UIAction> get_action(UIActionType type) const;

		size_t get_children(List<UINode>& children) const;
		UC_NODISCARD List<UINode> get_children() const;

		UC_NODISCARD Optional<UINode> get_next_sibling() const;
		UC_NODISCARD Optional<UINode> get_prev_sibling() const;

		// FIND //////////////////////////////////////////////////////////////////////
		UC_NODISCARD UINode find_by_type(UINodeType type) const;
		Size find_all_by_type(UINodeType type, List<UINode>& list) const;

		UC_NODISCARD UINode find_by_name(StringView name) const;
		Size find_all_by_name(StringView name, List<UINode>& list) const;

		static const UINode Empty;

	private:
		const UIDocument* _document;
		IndexType _index;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const UINode&);
}