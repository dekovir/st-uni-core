#pragma once
#include "unicore/remoteui/Document.hpp"

namespace unicore::remoteui
{
	class View
	{
	public:
		View();
		virtual ~View() = default;

		UC_NODISCARD const Shared<Document>& document() const { return _document; }
		void set_document(const Shared<Document>& document);

	protected:
		Shared<Document> _document;

		const std::function<void(const Element&)> _bind_create_node;
		const std::function<void(const Element&)> _bind_remove_node;
		const std::function<void(const Element&)> _bind_reorder_children;

		const std::function<void(const Element&, Attribute, const Optional<Variant>&)> _bind_set_attribute;

		virtual void on_rebuild() = 0;

		virtual void on_create_node(const Element& node) {}
		virtual void on_remove_node(const Element& node) {}
		virtual void on_reorder_children(const Element& node) {}
		virtual void on_set_attribute(const Element& node, Attribute type, const Optional<Variant>& value) {}
	};
}