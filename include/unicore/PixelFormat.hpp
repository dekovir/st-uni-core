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

	template<typename T>
	struct color_limits
	{
		static constexpr T min() noexcept { return std::numeric_limits<T>::min(); }
		static constexpr T max() noexcept { return std::numeric_limits<T>::max(); }
		static constexpr T range() noexcept { return std::numeric_limits<T>::max() - std::numeric_limits<T>::min(); }
	};

	template<>
	struct color_limits<float>
	{
		static constexpr float min() noexcept { return 0.0f; }
		static constexpr float max() noexcept { return 1.0f; }
		static constexpr float range() noexcept { return 1.0f; }
	};
}