#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<
		typename DataType,
		typename ComponentType>
	struct PixelFormat
	{
		using Data = DataType;
		using Element = ComponentType;

		static constexpr auto component_mask = std::numeric_limits<ComponentType>::max();

		uint8_t r_shift, g_shift, b_shift, a_shift;
	};

	// https://github.com/urho3d/Urho3D/blob/master/Source/Urho3D/Math/Color.cpp
	static constexpr PixelFormat<uint32_t, uint8_t> pixel_format_argb{ 16, 8, 0, 24 };
	static constexpr PixelFormat<uint32_t, uint8_t> pixel_format_abgr{ 0, 8, 16, 24 };

	static constexpr PixelFormat<uint32_t, uint8_t> pixel_format_rgba{ 24, 16, 8, 0 };
}