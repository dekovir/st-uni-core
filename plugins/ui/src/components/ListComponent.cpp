#include "unicore/ui/components/ListComponent.hpp"

namespace unicore::ui
{
	ListComponent::ListComponent(const Shared<ModelType>& model)
		: GroupComponent(UIGroupType::List)
		, _model(model)
	{
	}

	void ListComponent::on_mount()
	{
		const auto& size = _model->size();

		for (unsigned i = 0; i < size; i++)
			add(_model->get_at(i));

		GroupComponent::on_mount();
	}

	void ListComponent::on_dismount()
	{
		GroupComponent::on_dismount();

		_nodes.clear();
	}
}