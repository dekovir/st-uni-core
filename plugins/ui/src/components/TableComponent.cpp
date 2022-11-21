#include "unicore/ui/components/TableComponent.hpp"

namespace unicore::ui
{
	TableComponent::TableComponent(const Shared<TableComponentModel>& model)
		: GroupComponent(UIGroupType::Table)
		, _model(model)
	{
	}

	void TableComponent::on_mount()
	{
		GroupComponent::on_mount();

		const auto& size = _model->size();
		const auto col_count = size.x;
		const auto row_count = size.y;

		set_attribute(UIAttribute::Value, col_count);

		// Header row
		if (const auto row_layout = add(table_row()))
		{
			for (unsigned i = 0; i < col_count; i++)
				row_layout->add(TableHeaderComponent())->set_text(_model->get_header(i));
		}

		for (unsigned row = 0; row < row_count; row++)
		{
			if (const auto row_layout = add(table_row()))
			{
				for (unsigned col = 0; col < col_count; col++)
				{
					if (const auto cell_layout = row_layout->add(table_cell()))
					{
						const auto index = TableComponentModel::IndexType(col, row);
						auto component = _model->get_at(index);

						cell_layout->add(component);
						_cells[index] = component;
					}
				}
			}
		}
	}

	void TableComponent::on_dismount()
	{
		_cells.clear();

		GroupComponent::on_dismount();
	}
}