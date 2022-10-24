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
		using ValueType = T;
		static constexpr T max() noexcept { return std::numeric_limits<T>::max(); }
	};

	template<>
	struct color_limits<uint8_t>
	{
		using ValueType = unsigned;
		static constexpr uint8_t max() noexcept { return 0xFF; }
	};

	template<>
	struct color_limits<float>
	{
		using ValueType = float;
		static constexpr float max() noexcept { return 1.0f; }
	};

	struct color_limits_convert
	{
		template<typename TIn, typename TOut>
		static constexpr TOut component(TIn value)
		{
			if constexpr (std::is_same_v<TIn, TOut>) return value;

			if constexpr (std::is_floating_point_v<TIn> && std::is_floating_point_v<TOut>)
				return static_cast<TOut>(value);

			if constexpr (std::is_integral_v<TIn> && std::is_integral_v<TOut>)
				return static_cast<TOut>(value);

			// TODO: Optimize
			const auto normalized = static_cast<Double>(value) / static_cast<Double>(color_limits<TIn>::max());
			const auto converted = normalized * static_cast<Double>(color_limits<TOut>::max());
			return static_cast<TOut>(converted);
		}
	};
}