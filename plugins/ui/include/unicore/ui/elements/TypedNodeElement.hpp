#pragma once
#include "unicore/ui/elements/NodeElement.hpp"

namespace unicore::ui
{
	template<typename TTemplate,
		std::enable_if_t<std::is_base_of_v<NodeScheme, TTemplate>>* = nullptr>
	class TypedNodeElement : public NodeElement
	{
	public:
		Shared<NodeScheme> render() override
		{
			auto temp = ptr(TTemplate());
			apply_params(*temp);
			return temp;
		}

	protected:
		virtual void apply_params(TTemplate& item)
		{
			if (!_name.empty())
				item.set_params({ attr::Name(_name) });
			item.set_params({ attr::Visible(!_hidden) });
		}
	};
}