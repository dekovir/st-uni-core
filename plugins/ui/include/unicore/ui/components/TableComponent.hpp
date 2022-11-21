#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	using TableComponentModel = TableDataModel<Shared<Component>>;

	class TableHeaderComponent : public TypedGroupComponent<UIGroupType::TableHeader>
	{
	public:
		void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
		UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
	};

	class TableComponent : public GroupComponent
	{
	public:

		explicit TableComponent(const Shared<TableComponentModel>& model);

		UC_NODISCARD const Shared<TableComponentModel>& model() const { return _model; }

	protected:
		Shared<TableComponentModel> _model;
		Dictionary<TableComponentModel::IndexType, Shared<Component>, Vector2SortX<Size>> _cells;

		void on_mount() override;
		void on_dismount() override;
	};

	using table = TableComponent;
	using table_header = TableHeaderComponent;
}