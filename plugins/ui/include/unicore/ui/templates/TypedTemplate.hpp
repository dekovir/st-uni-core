#pragma once
#include "unicore/ui/templates/Template.hpp"

namespace unicore::ui
{
	template<UINodeTag Tag, typename... TKeys>
	class TypedTemplate : public Template
	{
	public:
		using VariantType = std::variant<attr::Uid, attr::Name, attr::Visible, TKeys...>;
		using Params = List<VariantType>;

		TypedTemplate() = default;

		void set_params(const Params& params)
		{
			for (const auto& value : params)
				apply_variant(value);
		}

		UINode create(UIDocument& document, const UINode& parent) override
		{
			return document.create_node(Tag, _options, parent);
		}

		Bool apply_to(UIDocument& document, const UINode& node) override
		{
			if (node.tag() != Tag)
				return false;

			return document.apply_options(node, _options);
		}

	protected:
		UINodeOptions _options;

		void apply_variant(const VariantType& value)
		{
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, attr::Uid>)
				_options.uid = arg.get();
			else if constexpr (std::is_same_v<T, attr::Name>)
				_options.name = arg.get();
			else if constexpr (std::is_same_v<T, attr::Visible>)
				_options.visible = arg.get();

			else if constexpr (std::is_same_v<T, attr::Type>)
				_options.attributes[UIAttribute::Type] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Value>)
				_options.attributes[UIAttribute::Value] = arg.get();

			else if constexpr (std::is_same_v<T, attr::Width>)
				_options.attributes[UIAttribute::Width] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Height>)
				_options.attributes[UIAttribute::Height] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Tooltip>)

				_options.attributes[UIAttribute::Tooltip] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Text>)
				_options.attributes[UIAttribute::Text] = arg.get();

			else if constexpr (std::is_same_v<T, attr::Step>)
				_options.attributes[UIAttribute::Step] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Min>)
				_options.attributes[UIAttribute::Min] = arg.get();
			else if constexpr (std::is_same_v<T, attr::Max>)
				_options.attributes[UIAttribute::Max] = arg.get();

			else if constexpr (std::is_same_v<T, action::OnClick>)
				_options.actions[UIActionType::OnClick] = arg.get();
			else if constexpr (std::is_same_v<T, action::OnChange>)
				_options.actions[UIActionType::OnChange] = arg.get();
				}, value);
		}
	};

	
}