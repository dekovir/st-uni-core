#pragma once
#include "unicore/ui/elements/Element.hpp"

namespace unicore::ui
{
#define UC_UI_ELEMENT_PROPERTY(name, type) \
	public: UC_NODISCARD sfinae::ConstRefType<type> name() const { return _ ## name; } \
	void set_ ## name(sfinae::ConstRefType<type> value) \
	{ if (_ ## name == value) return; _ ## name = value; rebuild(); } \
	protected: type _ ## name

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	class TypedElement : public Element
	{
		UC_UI_ELEMENT_PROPERTY(name, String);
		UC_UI_ELEMENT_PROPERTY(visible, Bool);
	public:
		Shared<Template> render() override
		{
			auto temp = ptr(T());
			apply_params(*temp);
			return temp;
		}

	protected:
		virtual void apply_params(T& item)
		{
			if (!_name.empty())
				item.set_params({ attr::Name(_name) });
			item.set_params({ attr::Visible(_visible) });
		}
	};
}