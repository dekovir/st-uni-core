#pragma once
#include "unicore/renderer/Font.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)

namespace unicore
{
	class StbEasyFont : public GeometryFont
	{
		UC_OBJECT(StbEasyFont, GeometryFont)
	public:
		StbEasyFont();

		UC_NODISCARD size_t get_system_memory_use() const override;

		UC_NODISCARD float get_height() const override { return _height; }

		UC_NODISCARD float calc_width(StringView32 text) const override;

		size_t generate(const Vector2f& position, StringView32 text,
			const Color4b& color, List<VertexColorQuad2>& quad_list) const override;

	protected:
		const float _height;

		static int internal_calc_height();
	};
}
#endif