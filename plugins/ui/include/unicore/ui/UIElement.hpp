#pragma once
#include "unicore/ui/UITemplate.hpp"

namespace unicore::ui
{
	class Element
	{
	public:
		virtual ~Element() = default;
		virtual Shared<Template> render() = 0;
	};

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

		virtual void apply_params(T& temp)
		{
			if (!_name.empty())
				temp.set_params({ attr::Name(_name) });
			temp.set_params({ attr::Visible(_visible) });
		}
	};

	class ButtonElement : public TypedElement<InputButton>
	{
		UC_OBJECT_EVENT(clicked, Bool);
	public:

	protected:
		String32 _text;

		void apply_params(InputButton& temp) override
		{
			TypedElement<InputButton>::apply_params(temp);

			if (!_text.empty())
				temp.set_params({ attr::Text(_text) });

			if (!_event_clicked.empty())
				temp.set_params({ action::OnClick([&] { _event_clicked.invoke(true); }) });
		}
	};
}