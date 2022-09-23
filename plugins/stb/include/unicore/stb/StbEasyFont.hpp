#pragma once
#include "unicore/Font.hpp"
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

		UC_NODISCARD float calc_width(WStringView text) const override;

		size_t generate(const Vector2f& position, WStringView text,
			const Color4b& color, List<VertexColorQuad2>& quad_list) const override;

	protected:
		const float _height;
	};
}
#endif