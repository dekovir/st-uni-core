#pragma once
#include "UINode.hpp"
#include "unicore/system/Index.hpp"
#include "unicore/system/Variant.hpp"

namespace unicore
{
	struct UINodeIndexTag {};
	using UINodeIndex = Index<UInt32, UINodeIndexTag>;
	static constexpr auto UINodeIndexInvalid = UINodeIndex(std::numeric_limits<UInt32>::max());

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
		UINode(UIDocument& document, const UINodeIndex& index);
		~UINode() = default;

		UINode(UINode const& other) noexcept = default;
		UINode(UINode&& other) = default;

		UINode& operator= (UINode const& other) noexcept;

		UINode& operator= (UINode&& other) noexcept;

		UC_NODISCARD UIDocument& document() const { return _document; }
		UC_NODISCARD UINodeIndex index() const { return _index; }

		UC_NODISCARD Bool valid() const;
		UC_NODISCARD UINodeType type() const;
		UC_NODISCARD Optional<UINode> parent() const;

		UC_NODISCARD const String& uid() const;

		UC_NODISCARD const String& name() const;
		void set_name(StringView name);

		UC_NODISCARD Bool visible() const;
		void set_visible(Bool value);

		UC_NODISCARD const UIAttributeDict& attributes() const;
		UC_NODISCARD const UIActionDict& actions() const;

		void set_attribute(UIAttributeType type, const Optional<Variant>& value);
		UC_NODISCARD Variant get_attribute(UIAttributeType type) const;

		UC_NODISCARD Variant get_value() const { return get_attribute(UIAttributeType::Value); }
		void set_value(const Optional<Variant>& value) { set_attribute(UIAttributeType::Value, value); }

		void set_action(UIActionType type, const Optional<UIAction>& value);
		UC_NODISCARD Optional<UIAction> get_action(UIActionType type) const;

		size_t get_children(List<UINode>& children) const;
		UC_NODISCARD List<UINode> get_children() const;

		UC_NODISCARD Optional<UINode> get_next_sibling() const;
		UC_NODISCARD Optional<UINode> get_prev_sibling() const;

		UINode create_child(UINodeType type, const UINodeOptions& options);
		UINode create_sibling(UINodeType type, const UINodeOptions& options);

		UINode duplicate();
		UINode duplicate_at(UINodeIndex parent = UINodeIndexInvalid);

		UC_NODISCARD Optional<UINode> find_child_by_name(StringView name) const;
		UC_NODISCARD Optional<UINode> find_child_by_name_recurse(StringView name) const;

		UC_NODISCARD size_t find_childs_by_name(StringView name, List<UINode>& list) const;
		UC_NODISCARD size_t find_childs_by_name_recurse(StringView name, List<UINode>& list) const;

	private:
		UIDocument& _document;
		UINodeIndex _index;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&);
	extern UNICODE_STRING_BUILDER_FORMAT(const UINode&);
}