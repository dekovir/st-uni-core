#pragma once
#include "unicore/system/Event.hpp"
#include "unicore/system/Index.hpp"
#include "unicore/math/Rect.hpp"

namespace unicore
{
	struct UINodeIndexTag {};
	using UINodeIndex = Index<UInt32, UINodeIndexTag>;

	using UIAttributeValue = Variant<
		Bool, Int, Int64, Double, String, String32,
		Vector2i, Vector2f, Recti, Rectf>;

	using UIAttributes = Dictionary<String, UIAttributeValue>;

	using UINodeActionValue = std::function<void()>;
	using UINodeActions = Dictionary<String, UINodeActionValue>;

	enum class UINodeType
	{
		Group,
		Text,
		Button,
	};

	struct UINodeConst
	{
		static constexpr StringView Text = "text";

		static constexpr StringView OnLoad = "load";
		static constexpr StringView OnClick = "click";
	};

	class UINode : protected std::enable_shared_from_this<UINode>
	{
		UC_OBJECT_EVENT(add_node, const Shared<UINode>&);
		UC_OBJECT_EVENT(remove_node, const Shared<UINode>&);
	public:
		explicit UINode(UINodeType type, const Weak<UINode>& parent = Weak<UINode>());

		UC_NODISCARD UINodeType type() const { return _type; }

		UC_NODISCARD const UIAttributes& attributes() const { return _attributes; }
		UC_NODISCARD const UINodeActions& actions() const { return _actions; }

		void set_attribute(StringView name, const Optional<UIAttributeValue>& value);
		UC_NODISCARD Optional<UIAttributeValue> get_attribute(StringView name) const;

		void set_action(StringView name, const Optional<UINodeActionValue>& value);
		UC_NODISCARD Optional<UINodeActionValue> get_action(StringView name) const;

		UC_NODISCARD const List<Shared<UINode>>& children() const { return _children; }
		Shared<UINode> create_node(UINodeType type);

	private:
		const UINodeType _type;
		UIAttributes _attributes{};
		UINodeActions _actions{};

		Weak<UINode> _parent;
		List<Shared<UINode>> _children;
	};
}