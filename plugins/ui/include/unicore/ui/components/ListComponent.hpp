#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	using ListComponentModel = DataModel1<Shared<Component>>;

	class ListComponent : public GroupComponent
	{
	public:
		explicit ListComponent(const Shared<ListComponentModel>& model);

		UC_NODISCARD const Shared<ListComponentModel>& model() const { return _model; }

	protected:
		Shared<ListComponentModel> _model;
		Dictionary<ListComponentModel::IndexType, Shared<Component>> _nodes;

		void on_mount() override;
		void on_dismount() override;
	};

	using list = ListComponent;
}