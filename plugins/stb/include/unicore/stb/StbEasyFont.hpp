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
		UC_NODISCARD Vector2f calc_size(StringView text) const override;

		size_t generate(const Vector2f& position, StringView text,
			List<VertexColorQuad2>& quad_list) const override;

	protected:
		const float _height;
	};
}
#endif