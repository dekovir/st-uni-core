#pragma once
#include "unicore/ui/schemes/SchemeNode.hpp"

namespace unicore::ui
{
	class InputSchemeNode : public SchemeNode
	{
	public:
		explicit InputSchemeNode(UIInputType type);

	protected:
		const UIInputType _type;

		void fill_optons(UINodeOptions& options) const override;
	};

	template<UIInputType Type, typename ... Keys>
	class InputSchemeNodeT : public InputSchemeNode, public SchemeNodeKeys<Keys...>
	{
	public:
		using Params = typename SchemeNodeKeys<Keys...>::Params;

		InputSchemeNodeT() : InputSchemeNode(Type) {}

		explicit InputSchemeNodeT(Params params)
			: InputSchemeNode(Type)
		{
			SchemeNodeKeys<Keys...>::_values.apply(params);
		}

	protected:
		void fill_optons(UINodeOptions& options) const override
		{
			SchemeNodeKeys<Keys...>::_values.fill(options);
			InputSchemeNode::fill_optons(options);
		}
	};

	using InputText = InputSchemeNodeT<UIInputType::Text,
		attr::Value, attr::OnChange>;
	using InputTextArea = InputSchemeNodeT<UIInputType::TextArea,
		attr::Value, attr::OnChange>;

	using InputToggle = InputSchemeNodeT<UIInputType::Toggle,
		attr::Value, attr::OnChange>;
	using InputRadio = InputSchemeNodeT<UIInputType::Radio,
		attr::Value, attr::OnChange>;

	using InputButton = InputSchemeNodeT<UIInputType::Button,
		attr::Text, attr::OnClick>;
	using InputItem = InputSchemeNodeT<UIInputType::Item,
		attr::Value, attr::Text, attr::OnClick, attr::OnChange>;

	using InputImage = InputSchemeNodeT<UIInputType::Image,
		attr::Value, attr::OnChange>;

	using InputInteger = InputSchemeNodeT<UIInputType::Integer,
		attr::Value, attr::Step, attr::Min, attr::Max, attr::OnChange>;
	using InputFloat = InputSchemeNodeT<UIInputType::Float,
		attr::Value, attr::Step, attr::Min, attr::Max, attr::OnChange>;

	using InputSliderI = InputSchemeNodeT<UIInputType::RangeI,
		attr::Value, attr::Min, attr::Max, attr::OnChange>;
	using InputSliderF = InputSchemeNodeT<UIInputType::RangeF,
		attr::Value, attr::Min, attr::Max, attr::OnChange>;

	using InputColor3 = InputSchemeNodeT<UIInputType::Color3,
		attr::Value, attr::OnChange>;
	using InputColor4 = InputSchemeNodeT<UIInputType::Color4,
		attr::Value, attr::OnChange>;
}