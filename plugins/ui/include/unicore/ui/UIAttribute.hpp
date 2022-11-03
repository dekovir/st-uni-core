#pragma once
#include "unicore/system/Variant.hpp"

namespace unicore
{
	enum class UIAttributeType
	{
		Value,
		Variant,
		Width,
		Height,
		Tooltip,
		Text,
		StepValue,
		MinValue,
		MaxValue,
	};

	enum class UIGroupValue
	{
		None,
		Vertical,
		Horizontal,
	};

	enum class UIInputVariant
	{
		Text,
		TextArea,
		Toggle,
		Radio,
		Button,
		Number,
		Range,
		Color3,
		Color4,
	};

	using UIAttributeDict = Dictionary<UIAttributeType, Variant>;
}