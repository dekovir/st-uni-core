#pragma once
#include "unicore/Font.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)

namespace unicore
{
	class EasyFont : public GeometryFont
	{
		UC_OBJECT(EasyFont, GeometryFont)
	public:
		UC_NODISCARD size_t get_system_memory_use() const override;
		size_t print_quads(const Vector2f& position, StringView text, List<VertexColor2>& quads) const override;
	};
}
#endif