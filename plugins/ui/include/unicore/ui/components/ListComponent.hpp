#pragma once
#include "unicore/ui/components/GroupComponent.hpp"
#include "unicore/resource/DataModel.hpp"

namespace unicore::ui
{
	class ListComponent : public GroupComponent
	{
	public:
		using ModelType = DataModel1<Shared<Component>>;

		explicit ListComponent(const Shared<ModelType>& model);

		UC_NODISCARD const Shared<ModelType>& model() const { return _model; }

	protected:
		Shared<ModelType> _model;
		Dictionary<ModelType::IndexType, Shared<Component>> _nodes;

		void on_mount() override;
		void on_dismount() override;
	};

	using list = ListComponent;
}