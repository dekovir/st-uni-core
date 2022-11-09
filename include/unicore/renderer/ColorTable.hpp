#pragma once

namespace unicore
{
	// Color names
	// https://www.w3schools.com/tags/ref_colornames.asp
	template<typename T>
	struct ColorTable
	{
		static constexpr auto Black = T::from_rgb(0x000000);
		static constexpr auto White = T::from_rgb(0xFFFFFF);

		static constexpr auto Red = T::from_rgb(0xFF0000);
		static constexpr auto Green = T::from_rgb(0x00FF00);
		static constexpr auto Blue = T::from_rgb(0x0000FF);

		static constexpr auto Cyan = T::from_rgb(0x00FFFF);
		static constexpr auto Magenta = T::from_rgb(0xFF00FF);
		static constexpr auto Yellow = T::from_rgb(0xFFFF00);

		// Shades of grey
		static constexpr auto DimGray = T::from_rgb(0x696969);
		static constexpr auto Gray = T::from_rgb(0x808080);
		static constexpr auto DarkGray = T::from_rgb(0xA9A9A9);
		static constexpr auto Silver = T::from_rgb(0xC0C0C0);
		static constexpr auto LightGray = T::from_rgb(0xD3D3D3);

		static constexpr auto LightSlateGray = T::from_rgb(0x778899);
		static constexpr auto SlateGray = T::from_rgb(0x708090);
		static constexpr auto DarkSlateGray = T::from_rgb(0x2F4F4F);

		ColorTable() = delete;
	};

	template<typename T>
	struct ColorTableWithalpha : ColorTable<T>
	{
		static constexpr auto MaxValue = T::MaxValue;

		static constexpr auto Clear = T(0, 0, 0, 0);
	};
}