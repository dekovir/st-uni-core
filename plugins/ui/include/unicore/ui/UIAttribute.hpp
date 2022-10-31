#pragma once
#include "unicore/system/Variant.hpp"

namespace unicore
{
	enum class UIAttributeType
	{
		Value,
		Width,
		Height,
		Tooltip,
		Text,
		MinValue,
		MaxValue,
	};

	enum class UIGroupValue
	{
		None,
		Vertical,
		Horizontal,
	};

	using UIAttributeDict = Dictionary<UIAttributeType, Variant>;
}