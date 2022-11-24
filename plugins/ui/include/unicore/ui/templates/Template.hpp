#pragma once
#include "unicore/ui/UIDocument.hpp"
#include "unicore/system/StrongType.hpp"

namespace unicore::ui
{
	class Template
	{
	public:
		virtual ~Template() = default;

		virtual UINode create(UIDocument& document, const UINode& parent) = 0;
		virtual Bool apply_to(UIDocument& document, const UINode& node) = 0;
	};

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
	}

	namespace action
	{
		UNICORE_MAKE_STRONG_TYPE(OnClick, UIActionDefault);
		UNICORE_MAKE_STRONG_TYPE(OnChange, UIAction);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(const T& element)
	{
		return std::make_shared<T>(element);
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ptr(T&& element)
	{
		return std::make_shared<T>(std::forward<T>(element));
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(const T& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Template, T>>* = nullptr>
	extern Shared<T> ref(T&& element, Shared<T>& ref)
	{
		ref = ptr(element);
		return ref;
	}
}