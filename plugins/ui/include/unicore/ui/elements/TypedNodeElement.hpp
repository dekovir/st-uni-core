#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	template<typename TScheme,
		std::enable_if_t<std::is_base_of_v<SchemeNode, TScheme>>* = nullptr>
	class TypedNodeElement : public Element
	{
		UC_UI_ELEMENT_PROPERTY(name, String);
		UC_UI_ELEMENT_PROPERTY(hidden, Bool);
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