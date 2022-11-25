#pragma once
#include "unicore/ui/schemes/SchemeNode.hpp"

namespace unicore::ui
{
	class VisualSchemeNode : public SchemeNode
	{
	public:
		explicit VisualSchemeNode(UIVisualType type);

	protected:
		UIVisualType _type;

		void fill_optons(UINodeOptions& options) const override;
	};

	template<UIVisualType Type, typename ... Keys>
	class VisualSchemeNodeT : public VisualSchemeNode, public SchemeNodeKeys<Keys...>
	{
	public:
		using Params = typename SchemeNodeKeys<Keys...>::Params;

		VisualSchemeNodeT() : VisualSchemeNode(Type) {}

		explicit VisualSchemeNodeT(Params params)
			: VisualSchemeNode(Type)
		{
			SchemeNodeKeys<Keys...>::_values.apply(params);
		}

	protected:
		void fill_optons(UINodeOptions& options) const override
		{
			SchemeNodeKeys<Keys...>::_values.fill(options);
			VisualSchemeNode::fill_optons(options);
		}
	};

	using VisualText = VisualSchemeNodeT<UIVisualType::Text, attr::Text>;
	using VisualColor = VisualSchemeNodeT<UIVisualType::Color, attr::Value>;
	using VisualImage = VisualSchemeNodeT<UIVisualType::Image, attr::Value>;
	using VisualProgress = VisualSchemeNodeT<UIVisualType::Progress, attr::Value, attr::Min, attr::Max>;
	using VisualSeparator = VisualSchemeNodeT<UIVisualType::Separator>;
	using VisualBullet = VisualSchemeNodeT<UIVisualType::Bullet>;
}