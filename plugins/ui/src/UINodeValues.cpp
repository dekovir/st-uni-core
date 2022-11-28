#include "unicore/ui/UINodeValues.hpp"

namespace unicore::ui
{
	UINodeValues::UINodeValues(Params params)
	{
		apply(params);
	}

	void UINodeValues::fill(UINodeOptions& options) const
	{
		options.uid = _uid;
		options.name = _name;
		options.attributes = _attributes;
		options.actions = _actions;
	}

	void UINodeValues::set(const VariantType& value)
	{
		std::visit([&](auto&& arg) { internal_set(arg); }, value);
	}

	void UINodeValues::apply(Params params)
	{
		for (const auto& value : params)
			set(value);
	}

	UINodeValuesAny::UINodeValuesAny(Params params)
	{
		apply(params);
	}

	void UINodeValuesAny::set(const attr::Any& value)
	{
		std::visit([&](auto&& arg) { internal_set(arg); }, value);
	}

	void UINodeValuesAny::apply(Params params)
	{
		for (const auto& value : params)
			set(value);
	}
}
