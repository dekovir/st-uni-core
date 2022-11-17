#pragma once

namespace unicore
{
	// Color names
	// https://www.w3schools.com/tags/ref_colornames.asp
	template<typename T>
	struct ColorTable
	{
		static constexpr auto AliceBlue = T::from_rgb(0xF0F8FF);
		static constexpr auto AntiqueWhite = T::from_rgb(0xFAEBD7);
		static constexpr auto Aqua = T::from_rgb(0x00FFFF);
		static constexpr auto Aquamarine = T::from_rgb(0x7FFFD4);
		static constexpr auto Azure = T::from_rgb(0xF0FFFF);
		static constexpr auto Beige = T::from_rgb(0xF5F5DC);
		static constexpr auto Bisque = T::from_rgb(0xFFE4C4);
		static constexpr auto Black = T::from_rgb(0x000000);
		static constexpr auto BlanchedAlmond = T::from_rgb(0xFFEBCD);
		static constexpr auto Blue = T::from_rgb(0x0000FF);
		static constexpr auto BlueViolet = T::from_rgb(0x8A2BE2);
		static constexpr auto Brown = T::from_rgb(0xA52A2A);
		static constexpr auto BurlyWood = T::from_rgb(0xDEB887);
		static constexpr auto CadetBlue = T::from_rgb(0x5F9EA0);
		static constexpr auto Chartreuse = T::from_rgb(0x7FFF00);
		static constexpr auto Chocolate = T::from_rgb(0xD2691E);
		static constexpr auto Coral = T::from_rgb(0xFF7F50);
		static constexpr auto CornflowerBlue = T::from_rgb(0x6495ED);
		static constexpr auto Cornsilk = T::from_rgb(0xFFF8DC);
		static constexpr auto Crimson = T::from_rgb(0xDC143C);
		static constexpr auto Cyan = T::from_rgb(0x00FFFF);
		static constexpr auto DarkBlue = T::from_rgb(0x00008B);
		static constexpr auto DarkCyan = T::from_rgb(0x008B8B);
		static constexpr auto DarkGoldenrod = T::from_rgb(0xB8860B);
		static constexpr auto DarkGray = T::from_rgb(0xA9A9A9);
		static constexpr auto DarkGreen = T::from_rgb(0x006400);
		static constexpr auto DarkKhaki = T::from_rgb(0xBDB76B);
		static constexpr auto DarkMagenta = T::from_rgb(0x8B008B);
		static constexpr auto DarkOliveGreen = T::from_rgb(0x556B2F);
		static constexpr auto DarkOrange = T::from_rgb(0xFF8C00);
		static constexpr auto DarkOrchid = T::from_rgb(0x9932CC);
		static constexpr auto DarkRed = T::from_rgb(0x8B0000);
		static constexpr auto DarkSalmon = T::from_rgb(0xE9967A);
		static constexpr auto DarkSeaGreen = T::from_rgb(0x8FBC8B);
		static constexpr auto DarkSlateBlue = T::from_rgb(0x483D8B);
		static constexpr auto DarkSlateGray = T::from_rgb(0x2F4F4F);
		static constexpr auto DarkTurquoise = T::from_rgb(0x00CED1);
		static constexpr auto DarkViolet = T::from_rgb(0x9400D3);
		static constexpr auto DeepPink = T::from_rgb(0xFF1493);
		static constexpr auto DeepSkyBlue = T::from_rgb(0x00BFFF);
		static constexpr auto DimGray = T::from_rgb(0x696969);
		static constexpr auto VeryDimGray = T::from_rgb(0x404040);
		static constexpr auto DodgerBlue = T::from_rgb(0x1E90FF);
		static constexpr auto Firebrick = T::from_rgb(0xB22222);
		static constexpr auto FloralWhite = T::from_rgb(0xFFFAF0);
		static constexpr auto ForestGreen = T::from_rgb(0x228B22);
		static constexpr auto Fuchsia = T::from_rgb(0xFF00FF);
		static constexpr auto Gainsboro = T::from_rgb(0xDCDCDC);
		static constexpr auto GhostWhite = T::from_rgb(0xF8F8FF);
		static constexpr auto Gold = T::from_rgb(0xFFD700);
		static constexpr auto Goldenrod = T::from_rgb(0xDAA520);
		static constexpr auto Gray = T::from_rgb(0x808080);
		static constexpr auto Green = T::from_rgb(0x000000);
		static constexpr auto GreenYellow = T::from_rgb(0xADFF2F);
		static constexpr auto Honeydew = T::from_rgb(0xF0FFF0);
		static constexpr auto HotPink = T::from_rgb(0xFF69B4);
		static constexpr auto IndianRed = T::from_rgb(0xCD5C5C);
		static constexpr auto Indigo = T::from_rgb(0x4B0082);
		static constexpr auto Ivory = T::from_rgb(0xFFFFF0);
		static constexpr auto Khaki = T::from_rgb(0xF0E68C);
		static constexpr auto Lavender = T::from_rgb(0xE6E6FA);
		static constexpr auto LavenderBlush = T::from_rgb(0xFFF0F5);
		static constexpr auto LawnGreen = T::from_rgb(0x7CFC00);
		static constexpr auto LemonChiffon = T::from_rgb(0xFFFACD);
		static constexpr auto LightBlue = T::from_rgb(0xADD8E6);
		static constexpr auto LightCoral = T::from_rgb(0xF08080);
		static constexpr auto LightCyan = T::from_rgb(0xE0FFFF);
		static constexpr auto LightGoldenrodYellow = T::from_rgb(0xFAFAD2);
		static constexpr auto LightGray = T::from_rgb(0xD3D3D3);
		static constexpr auto LightGreen = T::from_rgb(0x90EE90);
		static constexpr auto LightPink = T::from_rgb(0xFFB6C1);
		static constexpr auto LightSalmon = T::from_rgb(0xFFA07A);
		static constexpr auto LightSeaGreen = T::from_rgb(0x20B2AA);
		static constexpr auto LightSkyBlue = T::from_rgb(0x87CEFA);
		static constexpr auto LightSlateGray = T::from_rgb(0x778899);
		static constexpr auto LightSteelBlue = T::from_rgb(0xB0C4DE);
		static constexpr auto LightYellow = T::from_rgb(0xFFFFE0);
		static constexpr auto Lime = T::from_rgb(0x00FF00);
		static constexpr auto LimeGreen = T::from_rgb(0x32CD32);
		static constexpr auto Linen = T::from_rgb(0xFAF0E6);
		static constexpr auto Magenta = T::from_rgb(0xFF00FF);
		static constexpr auto Maroon = T::from_rgb(0x800000);
		static constexpr auto MediumAquamarine = T::from_rgb(0x66CDAA);
		static constexpr auto MediumBlue = T::from_rgb(0x0000CD);
		static constexpr auto MediumOrchid = T::from_rgb(0xBA55D3);
		static constexpr auto MediumPurple = T::from_rgb(0x9370DB);
		static constexpr auto MediumSeaGreen = T::from_rgb(0x3CB371);
		static constexpr auto MediumSlateBlue = T::from_rgb(0x7B68EE);
		static constexpr auto MediumSpringGreen = T::from_rgb(0x00FA9A);
		static constexpr auto MediumTurquoise = T::from_rgb(0x48D1CC);
		static constexpr auto MediumVioletRed = T::from_rgb(0xC71585);
		static constexpr auto MidnightBlue = T::from_rgb(0x191970);
		static constexpr auto MintCream = T::from_rgb(0xF5FFFA);
		static constexpr auto MistyRose = T::from_rgb(0xFFE4E1);
		static constexpr auto Moccasin = T::from_rgb(0xFFE4B5);
		static constexpr auto NavajoWhite = T::from_rgb(0xFFDEAD);
		static constexpr auto Navy = T::from_rgb(0x000080);
		static constexpr auto OldLace = T::from_rgb(0xFDF5E6);
		static constexpr auto Olive = T::from_rgb(0x808000);
		static constexpr auto OliveDrab = T::from_rgb(0x6B8E23);
		static constexpr auto Orange = T::from_rgb(0xFFA500);
		static constexpr auto OrangeRed = T::from_rgb(0xFF4500);
		static constexpr auto Orchid = T::from_rgb(0xDA70D6);
		static constexpr auto PaleGoldenrod = T::from_rgb(0xEEE8AA);
		static constexpr auto PaleGreen = T::from_rgb(0x98FB98);
		static constexpr auto PaleTurquoise = T::from_rgb(0xAFEEEE);
		static constexpr auto PaleVioletRed = T::from_rgb(0xDB7093);
		static constexpr auto PapayaWhip = T::from_rgb(0xFFEFD5);
		static constexpr auto PeachPuff = T::from_rgb(0xFFDAB9);
		static constexpr auto Peru = T::from_rgb(0xCD853F);
		static constexpr auto Pink = T::from_rgb(0xFFC0CB);
		static constexpr auto Plum = T::from_rgb(0xDDA0DD);
		static constexpr auto PowderBlue = T::from_rgb(0xB0E0E6);
		static constexpr auto Purple = T::from_rgb(0x800080);
		static constexpr auto Red = T::from_rgb(0xFF0000);
		static constexpr auto RosyBrown = T::from_rgb(0xBC8F8F);
		static constexpr auto RoyalBlue = T::from_rgb(0x4169E1);
		static constexpr auto SaddleBrown = T::from_rgb(0x8B4513);
		static constexpr auto Salmon = T::from_rgb(0xFA8072);
		static constexpr auto SandyBrown = T::from_rgb(0xF4A460);
		static constexpr auto SeaGreen = T::from_rgb(0x2E8B57);
		static constexpr auto SeaShell = T::from_rgb(0xFFF5EE);
		static constexpr auto Sienna = T::from_rgb(0xA0522D);
		static constexpr auto Silver = T::from_rgb(0xC0C0C0);
		static constexpr auto SkyBlue = T::from_rgb(0x87CEEB);
		static constexpr auto SlateBlue = T::from_rgb(0x6A5ACD);
		static constexpr auto SlateGray = T::from_rgb(0x708090);
		static constexpr auto Snow = T::from_rgb(0xFFFAFA);
		static constexpr auto SpringGreen = T::from_rgb(0x00FF7F);
		static constexpr auto SteelBlue = T::from_rgb(0x4682B4);
		static constexpr auto Tan = T::from_rgb(0xD2B48C);
		static constexpr auto Teal = T::from_rgb(0x008080);
		static constexpr auto Thistle = T::from_rgb(0xD8BFD8);
		static constexpr auto Tomato = T::from_rgb(0xFF6347);
		static constexpr auto Turquoise = T::from_rgb(0x40E0D0);
		static constexpr auto Violet = T::from_rgb(0xEE82EE);
		static constexpr auto Wheat = T::from_rgb(0xF5DEB3);
		static constexpr auto White = T::from_rgb(0xFFFFFF);
		static constexpr auto WhiteSmoke = T::from_rgb(0xF5F5F5);
		static constexpr auto Yellow = T::from_rgb(0xFFFF00);
		static constexpr auto YellowGreen = T::from_rgb(0x9ACD32);

		ColorTable() = delete;
	};

	template<typename T>
	struct ColorTableWithalpha : ColorTable<T>
	{
		static constexpr auto MaxValue = T::MaxValue;

		static constexpr auto Clear = T(0, 0, 0, 0);
		static constexpr auto Transparent = T(0, 0, 0, 0);
	};
}