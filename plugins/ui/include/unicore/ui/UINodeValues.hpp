#pragma once
#include "unicore/ui/UINode.hpp"
#include "unicore/system/StrongType.hpp"

namespace unicore::ui
{
	namespace attr
	{
		UNICORE_MAKE_STRONG_TYPE(Uid, String);
		UNICORE_MAKE_STRONG_TYPE(Name, String);
		UNICORE_MAKE_STRONG_TYPE(Visible, Bool);

		UNICORE_MAKE_STRONG_TYPE(Type, UInt8);
		UNICORE_MAKE_STRONG_TYPE(Value, Variant);

		UNICORE_MAKE_STRONG_TYPE(Width, Variant);
		UNICORE_MAKE_STRONG_TYPE(Height, Variant);

		UNICORE_MAKE_STRONG_TYPE(Tooltip, String32);
		UNICORE_MAKE_STRONG_TYPE(Text, String32);

		UNICORE_MAKE_STRONG_TYPE(Step, Variant);
		UNICORE_MAKE_STRONG_TYPE(Min, Variant);
		UNICORE_MAKE_STRONG_TYPE(Max, Variant);

		UNICORE_MAKE_STRONG_TYPE(OnClick, UIActionDefault);
		UNICORE_MAKE_STRONG_TYPE(OnChange, UIAction);

		using Any = StdVariant<
			Uid, Name, Visible, Type,
			Value, Width, Height, Tooltip, Text, Step, Min, Max,
			OnClick, OnChange>;
	}

	// SFINAE ////////////////////////////////////////////////////////////////////
	template <class T>
	inline constexpr bool is_attr_v =
		std::is_same_v<T, attr::Uid> ||
		std::is_same_v<T, attr::Name> ||
		std::is_same_v<T, attr::Visible> ||
		std::is_same_v<T, attr::Type> ||
		std::is_same_v<T, attr::Value> ||
		std::is_same_v<T, attr::Width> ||
		std::is_same_v<T, attr::Height> ||
		std::is_same_v<T, attr::Tooltip> ||
		std::is_same_v<T, attr::Text> ||
		std::is_same_v<T, attr::Step> ||
		std::is_same_v<T, attr::Min> ||
		std::is_same_v<T, attr::Max> ||
		std::is_same_v<T, attr::OnClick> ||
		std::is_same_v<T, attr::OnChange>;

	template<typename ... T>
	inline constexpr Bool all_is_attr_v = (... && is_attr_v<T>);

	// UINodeValues //////////////////////////////////////////////////////////////
	class UINodeValues
	{
	public:
		using VariantType = std::variant<attr::Uid, attr::Name, attr::Visible>;
		using Params = std::initializer_list<VariantType>;

		UINodeValues() = default;

		UINodeValues(Params params)
		{
			apply(params);
		}

		void fill(UINodeOptions& options) const
		{
			options.uid = _uid;
			options.name = _name;
			options.visible = _visible;
			options.attributes = _attributes;
			options.actions = _actions;
		}

		void set(const VariantType& value)
		{
			std::visit([&](auto&& arg) { internal_set(arg); }, value);
		}

		void apply(Params params)
		{
			for (const auto& value : params)
				set(value);
		}

	protected:
		String _uid;
		String _name;
		Bool _visible = true;

		UIAttributeDict _attributes = {};
		UIActionDict _actions = {};

		void internal_set(const attr::Uid& value) { _uid = value.get(); }
		void internal_set(const attr::Name& value) { _name = value.get(); }
		void internal_set(const attr::Visible& value) { _visible = value.get(); }
		void internal_set(const attr::Type& value) { _attributes[UIAttribute::Type] = value.get(); }
		void internal_set(const attr::Value& value) { _attributes[UIAttribute::Value] = value.get(); }
		void internal_set(const attr::Width& value) { _attributes[UIAttribute::Width] = value.get(); }
		void internal_set(const attr::Height& value) { _attributes[UIAttribute::Height] = value.get(); }
		void internal_set(const attr::Tooltip& value) { _attributes[UIAttribute::Tooltip] = value.get(); }
		void internal_set(const attr::Text& value) { _attributes[UIAttribute::Text] = value.get(); }
		void internal_set(const attr::Step& value) { _attributes[UIAttribute::Step] = value.get(); }
		void internal_set(const attr::Min& value) { _attributes[UIAttribute::Min] = value.get(); }
		void internal_set(const attr::Max& value) { _attributes[UIAttribute::Max] = value.get(); }
		void internal_set(const attr::OnClick& value) { _actions[UIActionType::OnClick] = value.get(); }
		void internal_set(const attr::OnChange& value) { _actions[UIActionType::OnChange] = value.get(); }
	};

	class UINodeValuesAny : public UINodeValues
	{
	public:
		using Params = std::initializer_list<attr::Any>;

		UINodeValuesAny() = default;

		UINodeValuesAny(Params params)
		{
			apply(params);
		}

		void set(const attr::Any& value)
		{
			std::visit([&](auto&& arg) { internal_set(arg); }, value);
		}

		void apply(Params params)
		{
			for (const auto& value : params)
				set(value);
		}
	};

	template<typename... TKeys>
	class UINodeValuesT : public UINodeValues
	{
	public:
		using VariantType = std::variant<attr::Uid, attr::Name, attr::Visible, TKeys...>;
		using Params = std::initializer_list<VariantType>;

		UINodeValuesT() = default;

		UINodeValuesT(Params params)
		{
			apply(params);
		}

		void set(const VariantType& value)
		{
			std::visit([&](auto&& arg) { internal_set(arg); }, value);
		}

		void apply(Params params)
		{
			for (const auto& value : params)
				set(value);
		}
	};
}