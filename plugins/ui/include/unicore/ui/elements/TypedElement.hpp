#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	class TypedElement : public Element
	{
	public:
		Shared<Template> render() override
		{
			auto temp = ptr(T());
			apply_params(*temp);
			return temp;
		}

	protected:
		String _name;
		Bool _visible = true;

		virtual void apply_params(T& item)
		{
			if (!_name.empty())
				item.set_params({ attr::Name(_name) });
			item.set_params({ attr::Visible(_visible) });
		}
	};
}