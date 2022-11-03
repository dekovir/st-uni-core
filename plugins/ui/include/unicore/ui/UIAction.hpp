#pragma once
#include "unicore/system/Variant.hpp"

namespace unicore
{
	class UINode;

	using UIActionDefault = std::function<void()>;
	using UIActionNodeDefault = std::function<void(const UINode&)>;

	using UIActionValue = std::function<void(const Variant&)>;
	using UIActionNodeValue = std::function<void(const UINode&, const Variant&)>;

	using UIAction = StdVariant<UIActionDefault, UIActionNodeDefault, UIActionValue, UIActionNodeValue>;

	enum class UIActionType : uint8_t
	{
		OnClick,
		OnChange, // Value changed
		OnMouseEnter, // Mouse cursor enters node
		OnMouseLeave, // Mouse cursor leaves node
	};
	using UIActionDict = Dictionary<UIActionType, UIAction>;
}