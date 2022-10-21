#pragma once
#include "unicore/system/Index.hpp"
#include "unicore/system/Variant.hpp"

namespace unicore
{
	struct UINodeIndexTag {};
	using UINodeIndex = Index<UInt32, UINodeIndexTag>;
	static constexpr auto UINodeIndexInvalid = UINodeIndex(std::numeric_limits<UInt32>::max());

	enum class UIAttributeType
	{
		Uid,
		Name,
		//Style,
		Value,
		MinValue,
		MaxValue,
	};

	using UIAttributes = Dictionary<UIAttributeType, Variant>;

	using UIAction = std::function<void()>;

	enum class UIActionType
	{
		OnLoad,
		OnClick,
		OnChange,
	};
	using UINodeActions = Dictionary<UIActionType, UIAction>;

	enum class UINodeType
	{
		None,
		Window,
		Group,
		Text,
		Button,
		Input,
		Slider,
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

		UC_NODISCARD const UIAttributes& attributes() const;
		UC_NODISCARD const UINodeActions& actions() const;

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

	private:
		UIDocument& _document;
		UINodeIndex _index;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const UINodeIndex&);
}