#pragma once
#include "unicore/system/Variant.hpp"

namespace unicore
{
	class UINode;

	using UIActionDefault = Action<void>;
	using UIActionValue = Action<const Variant&>;
	using UIAction = StdVariant<UIActionDefault, UIActionValue>;

	enum class UIActionType : uint8_t
	{
		OnClick,
		OnChange, // Value changed
		OnMouseEnter, // Mouse cursor enters node TODO: Remove?
		OnMouseLeave, // Mouse cursor leaves node TODO: Remove?
	};
	using UIActionDict = Dictionary<UIActionType, UIAction>;
}