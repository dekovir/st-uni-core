#pragma once
#include "unicore/ui/elements/NodeElement.hpp"

namespace unicore::ui
{
	template<typename TScheme,
		std::enable_if_t<std::is_base_of_v<SchemeNode, TScheme>>* = nullptr>
	class TypedNodeElement : public NodeElement
	{
	public:
		Shared<SchemeNode> render() override
		{
			auto temp = ptr(TScheme());
			apply_params(*temp);
			return temp;
		}

	protected:
		virtual void apply_params(TScheme& item)
		{
			if (!_name.empty())
				item.set_params({ attr::Name(_name) });
			item.set_params({ attr::Hidden(_hidden) });
		}
	};
}