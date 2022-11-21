#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	class TableComponent : public GroupComponent
	{
	public:
		using ModelType = TableDataModel<Shared<Component>>;

		explicit TableComponent(const Shared<ModelType>& model);

		UC_NODISCARD const Shared<ModelType>& model() const { return _model; }

	protected:
		class Header : public TypedGroupComponent<UIGroupType::TableHeader>
		{
		public:
			void set_text(StringView32 text) { set_attribute(UIAttribute::Text, text); }
			UC_NODISCARD String32 text() const { return get_attribute(UIAttribute::Text).get_string32(); }
		};

		Shared<ModelType> _model;
		Dictionary<ModelType::IndexType, Shared<Component>, Vector2SortX<Size>> _cells;

		void on_mount() override;
		void on_dismount() override;
	};

	using table = TableComponent;
}