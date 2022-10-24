#pragma once
#include "UINode.hpp"
#include "unicore/system/Index.hpp"
#include "unicore/system/Variant.hpp"

namespace unicore
{
	struct UINodeIndexTag {};
	using UINodeIndex = Index<UInt32, UINodeIndexTag>;
	static constexpr auto UINodeIndexInvalid = UINodeIndex(std::numeric_limits<UInt32>::max());

	enum class UIAttributeType
	{
		Uid, // Must be unique for UIDocument
		Name,
		//Style,
		Layout,
		Value,
		MinValue,
		MaxValue,
	};

	enum class UILayout
	{
		None,
		Vertical,
		Horizontal,
	};

	class UINode;

	using UIAttributeDict = Dictionary<UIAttributeType, Variant>;

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

	enum class UINodeType
	{
		None,
		Window,
		Group,
		Text,
		Button,
		Input,
		Slider,
		Toggle,
	};

	class UIDocument;

	class UINode
	{
	public:
		UINode(UIDocument& document, const UINodeIndex& index);

		UC_NODISCARD UIDocument& document() const { return _document; }
		UC_NODISCARD UINodeIndex index() const { return _index; }

		UC_NODISCARD Bool valid() const;
		UC_NODISCARD UINodeType type() const;
		UC_NODISCARD Optional<UINode> parent() const;

		UC_NODISCARD const UIAttributeDict& attributes() const;
		UC_NODISCARD const UIActionDict& actions() const;

		void set_attribute(UIAttributeType type, const Optional<Variant>& value);
		UC_NODISCARD Variant get_attribute(UIAttributeType type) const;

		void set_action(UIActionType type, const Optional<UIAction>& value);
		UC_NODISCARD Optional<UIAction> get_action(UIActionType type) const;

		size_t get_children(List<UINode>& children) const;
		UC_NODISCARD List<UINode> get_children() const;

		UC_NODISCARD Optional<UINode> get_next_sibling() const;
		UC_NODISCARD Optional<UINode> get_prev_sibling() const;

		UINode create_child(UINodeType type);
		UINode create_sibling(UINodeType type);

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