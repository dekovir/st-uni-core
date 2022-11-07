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

	enum class UIGroupVariant
	{
		Vertical,
		Horizontal,
		List,
		Flex,
		Popup,
		Modal,
	};

	enum class UIInputVariant
	{
		Text,
		TextArea,
		Toggle,
		Radio,
		Button,
		Image,
		Number,
		Range,
		Vector2,
		Vector3,
		Color3,
		Color4,
	};

	using UIAttributeDict = Dictionary<UIAttributeType, Variant>;
}