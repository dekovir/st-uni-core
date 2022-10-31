#pragma once
#include "unicore/ui/UIDocument.hpp"

namespace unicore
{
	class UIView
	{
	public:
		UIView();
		virtual ~UIView() = default;

		UC_NODISCARD const Shared<UIDocument>& document() const { return _document; }
		void set_document(const Shared<UIDocument>& document);

	protected:
		Shared<UIDocument> _document;

		const std::function<void(const UINode&)> _bind_create_node;
		const std::function<void(const UINode&)> _bind_remove_node;

		const std::function<void(const UINode&, StringView)> _bind_set_name;
		const std::function<void(const UINode&, StringView)> _bind_set_style;
		const std::function<void(const UINode&, Bool)> _bind_set_visible;
		const std::function<void(const UINode&, UIAttributeType, const Optional<Variant>&)> _bind_set_attribute;
		const std::function<void(const UINode&, UIActionType, const Optional<UIAction>&)> _bind_set_action;

		virtual void on_rebuild() = 0;
		virtual void on_create_node(const UINode& node) {}
		virtual void on_remove_node(const UINode& node) {}

		virtual void on_set_name(const UINode& node, StringView value) {}
		virtual void on_set_style(const UINode& node, StringView value) {}
		virtual void on_set_visible(const UINode& node, Bool value) {}

		virtual void on_set_attribute(const UINode& node, UIAttributeType type, const Optional<Variant>& value) {}
		virtual void on_set_action(const UINode& node, UIActionType type, const Optional<UIAction>& value) {}

		static UIGroupValue get_group_value(const Variant& value, UIGroupValue default_value = UIGroupValue::None);
	};
}