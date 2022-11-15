#include "unicore/ui/components/TableComponent.hpp"

namespace unicore::ui
{
	TableComponent::TableComponent(
		const Shared<TableDataModel<Shared<Component>>>& model)
		: GroupComponent(UIGroupType::Table)
		, _model(model)
	{
	}

	void TableComponent::on_mount()
	{
		const auto row_count = _model->size();
		const Size col_count = _model->col_count();

		set_attribute(UIAttribute::Value, col_count);

		// Header row
		if (const auto row_layout = add(table_row()))
		{
			for (unsigned i = 0; i < col_count; i++)
				row_layout->add(Header())->set_text(_model->get_header(i));
		}

		for (unsigned row = 0; row < row_count; row++)
		{
			if (const auto row_layout = add(table_row()))
			{
				for (unsigned col = 0; col < col_count; col++)
				{
					if (const auto cell_layout = row_layout->add(table_cell()))
					{
						auto component = _model->get_at(row, col);

						cell_layout->add(component);
						_cells[Vector2i(row, col)] = component;
					}
				}
			}
		}
	}

	void TableComponent::on_dismount()
	{
		_cells.clear();
	}
}