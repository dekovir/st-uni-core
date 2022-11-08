#pragma once
#include "unicore/system/Variant.hpp"

namespace unicore
{
	enum class UIAttributeType
	{
		Type,
		Value,
		Width,
		Height,
		Tooltip,
		Text,
		StepValue,
		MinValue,
		MaxValue,
	};

	enum class UIGroupType
	{
		Vertical,
		Horizontal,
		Child,
		List,
		Flex,
		Popup,
		Modal,
	};

	enum class UIInputType
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