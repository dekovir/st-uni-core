#pragma once
#include "unicore/ui/UINode.hpp"
#include "unicore/system/StrongType.hpp"

namespace unicore
{
	namespace test
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

	class UINodeValues
	{
	public:
		using VariantType = std::variant<test::Uid, test::Name, test::Visible>;
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

		void internal_set(const test::Uid& value) { _uid = value.get(); }
		void internal_set(const test::Name& value) { _name = value.get(); }
		void internal_set(const test::Visible& value) { _visible = value.get(); }
		void internal_set(const test::Type& value) { _attributes[UIAttribute::Type] = value.get(); }
		void internal_set(const test::Value& value) { _attributes[UIAttribute::Value] = value.get(); }
		void internal_set(const test::Width& value) { _attributes[UIAttribute::Width] = value.get(); }
		void internal_set(const test::Height& value) { _attributes[UIAttribute::Height] = value.get(); }
		void internal_set(const test::Tooltip& value) { _attributes[UIAttribute::Tooltip] = value.get(); }
		void internal_set(const test::Text& value) { _attributes[UIAttribute::Text] = value.get(); }
		void internal_set(const test::Step& value) { _attributes[UIAttribute::Step] = value.get(); }
		void internal_set(const test::Min& value) { _attributes[UIAttribute::Min] = value.get(); }
		void internal_set(const test::Max& value) { _attributes[UIAttribute::Max] = value.get(); }
		void internal_set(const test::OnClick& value) { _actions[UIActionType::OnClick] = value.get(); }
		void internal_set(const test::OnChange& value) { _actions[UIActionType::OnChange] = value.get(); }
	};

	class UINodeValuesAny : public UINodeValues
	{
	public:
		using Params = std::initializer_list<test::Any>;

		UINodeValuesAny() = default;

		UINodeValuesAny(Params params)
		{
			apply(params);
		}

		void set(const test::Any& value)
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
		using VariantType = std::variant<test::Uid, test::Name, test::Visible, TKeys...>;
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