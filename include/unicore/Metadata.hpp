#pragma once
#include "unicore/ConstString.hpp"

namespace unicore::Metadata
{
	template<typename ClassType>
	struct Type
	{
	};

	template<typename ClassType, typename T>
	struct PropertyInfo
	{
		using DataType = Type<T>;

		const StringView name;

		explicit constexpr PropertyInfo(StringView name_) noexcept
			: name(name_) {}
	};

	struct Attribute
	{
	};

	struct CommentAttribute : Attribute
	{
		StringView text;

		explicit constexpr CommentAttribute(StringView text_) noexcept : text(text_) {}
	};

	enum class AttributeTargets
	{
		Class = 1,
		Property = 2,
		Method = 4,
		All = Class | Property | Method
	};

	struct AttributeUsage : Attribute
	{
		AttributeTargets targets;
		bool allow_multiple;
		//bool inherited = false;

		explicit constexpr AttributeUsage(
			AttributeTargets targets_ = AttributeTargets::All,
			bool allow_multiple_ = false) noexcept
			: targets(targets_), allow_multiple(allow_multiple_)
		{}
	};

	template<typename T>
	constexpr bool is_attribute_v = std::is_base_of_v<Attribute, T>;
}

#define UNICORE_METADATA_REGISTER(TypeName, ...) \
		template<> struct Metadata::Type<TypeName> { \
		static constexpr auto name = make_const_string(#TypeName); \
		static constexpr auto members = std::make_tuple( __VA_ARGS__ ); \
		static constexpr auto members_count = std::tuple_size_v<decltype(members)>; \
	}