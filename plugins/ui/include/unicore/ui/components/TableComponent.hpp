#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/ui/UIDataModel.hpp"

namespace unicore::ui
{
	class TableComponent : public GroupComponent
	{
	public:
		class Header : public TypedGroupComponent<UIGroupType::TableHeader>
		{
		public:
			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
		};

		explicit TableComponent(const Shared<TableDataModel<Shared<Component>>>& model);

	protected:
		Shared<TableDataModel<Shared<Component>>> _model;
		Dictionary<Vector2i, Shared<Component>, Vector2SortX<int>> _cells;

		void on_mount() override;
		void on_dismount() override;
	};

	using table = TableComponent;
}